#include "handlers/recipes/create_recipe.hpp"

#include <cstdint>
#include <span>
#include <vector>

#include <docs/api/openapi-schema.hpp>
#include <userver/server/handlers/exceptions.hpp>
#include <userver/server/http/http_status.hpp>
#include <userver/storages/postgres/exceptions.hpp>

#include "auth/user_id.hpp"
#include "handlers/recipes/ingredient_errors.hpp"
#include "handlers/recipes/recipe_data.hpp"
#include "handlers/recipes/recipe_json.hpp"
#include "utils/image.hpp"

namespace foodgram::handlers::recipes {

using userver::server::handlers::ClientError;
using userver::server::handlers::ExternalBody;
using userver::server::handlers::ServiceErrorCode;
using userver::server::http::HttpRequest;
using userver::server::http::HttpStatus;
using userver::server::request::RequestContext;
using userver::storages::postgres::ClusterHostType;
using userver::storages::postgres::IntegrityConstraintViolation;
namespace json = userver::formats::json;

json::Value CreateRecipe::HandleRequestJsonThrow(
    const HttpRequest& request,
    const json::Value& request_json,
    RequestContext& context
) const {
    const auto body = ParseBody<schemas::RecipeCreate>(request_json);
    const auto user_id = *foodgram::auth::GetUserId(context);
    const auto image = utils::ParseImage(body.image, config_source_.GetSnapshot());

    std::vector<std::int64_t> ingredient_ids;
    std::vector<std::int32_t> amounts;
    ingredient_ids.reserve(body.ingredients.size());
    amounts.reserve(body.ingredients.size());
    for (const auto& ingredient : body.ingredients) {
        ingredient_ids.push_back(ingredient.id);
        amounts.push_back(ingredient.amount);
    }

    const auto image_url = media_.PutRecipeImage(image);

    std::vector<CreateRecipeRow> rows;
    try {
        rows = pg_client_.CreateRecipe(
            ClusterHostType::kMaster,
            user_id,
            body.name,
            image_url,
            body.text,
            body.cooking_time,
            ingredient_ids,
            amounts
        );
    } catch (const IntegrityConstraintViolation& error) {
        if (const auto ingredient_error = IngredientErrorMessage(error)) {
            throw ClientError(
                ServiceErrorCode{ingredient_error->code},
                ExternalBody{ingredient_error->message}
            );
        }
        throw;
    }

    const auto& recipe = rows.front();
    const std::vector<std::int64_t> recipe_ids{recipe.id.value()};
    const auto ingredients = LoadIngredients(pg_client_, ClusterHostType::kMaster, recipe_ids);

    request.SetResponseStatus(HttpStatus::kCreated);
    return RecipeToJson(recipe, std::span<const GetRecipeIngredientsRow>{ingredients});
}

}  // namespace foodgram::handlers::recipes
