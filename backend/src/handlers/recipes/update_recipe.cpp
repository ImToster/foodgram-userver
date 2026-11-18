#include "handlers/recipes/update_recipe.hpp"

#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include <docs/api/openapi-schema.hpp>
#include <foodgram/sql_queries.hpp>
#include <userver/server/handlers/exceptions.hpp>
#include <userver/storages/postgres/exceptions.hpp>
#include <userver/storages/postgres/io/row_types.hpp>

#include "auth/user_id.hpp"
#include "handlers/recipes/ingredient_errors.hpp"
#include "handlers/recipes/recipe_data.hpp"
#include "handlers/recipes/recipe_json.hpp"
#include "utils/image.hpp"
#include "utils/request.hpp"

namespace foodgram::handlers::recipes {

using userver::server::handlers::ClientError;
using userver::server::handlers::CustomHandlerException;
using userver::server::handlers::ExternalBody;
using userver::server::handlers::ResourceNotFound;
using userver::server::handlers::ServiceErrorCode;
using userver::server::http::HttpRequest;
using userver::server::request::RequestContext;
using userver::storages::postgres::ClusterHostType;
using userver::storages::postgres::IntegrityConstraintViolation;
namespace json = userver::formats::json;

json::Value UpdateRecipe::HandleRequestJsonThrow(
    const HttpRequest& request,
    const json::Value& request_json,
    RequestContext& context
) const {
    const auto id = utils::ParsePathId(request.GetPathArg("id"));
    const auto user_id = *foodgram::auth::GetUserId(context);
    const auto body = ParseBody<schemas::RecipeUpdate>(request_json);

    const auto current = FindRecipe(pg_client_, ClusterHostType::kMaster, id, user_id);
    if (current.author_id.value() != user_id) {
        throw CustomHandlerException(
            HandlerErrorCode::kForbidden, ExternalBody{"Only the author can modify the recipe"}
        );
    }

    std::vector<std::int64_t> ingredient_ids;
    std::vector<std::int32_t> amounts;
    if (body.ingredients) {
        ingredient_ids.reserve(body.ingredients->size());
        amounts.reserve(body.ingredients->size());
        for (const auto& ingredient : *body.ingredients) {
            ingredient_ids.push_back(ingredient.id);
            amounts.push_back(ingredient.amount);
        }
    }

    auto image_url = current.image.value();
    if (body.image) {
        const auto image = utils::ParseImage(*body.image, config_source_.GetSnapshot());
        image_url = media_.PutRecipeImage(image);
    }

    std::optional<UpdateRecipeRow> updated;
    std::vector<GetRecipeIngredientsRow> ingredients;

    try {
        auto transaction = cluster_->Begin("update_recipe", ClusterHostType::kMaster, {});

        updated = transaction.Execute(
            foodgram::sql::kUpdateRecipe,
            id,
            body.name.value_or(current.name.value()),
            image_url,
            body.text.value_or(current.text.value()),
            body.cooking_time.value_or(current.cooking_time.value()),
            user_id
        ).AsOptionalSingleRow<UpdateRecipeRow>(userver::storages::postgres::kRowTag);

        if (!updated) {
            throw ResourceNotFound(ExternalBody{"Recipe not found"});
        }

        if (body.ingredients) {
            transaction.Execute(foodgram::sql::kDeleteRecipeIngredients, id);
            ingredients = transaction.Execute(
                foodgram::sql::kAddRecipeIngredients, id, ingredient_ids, amounts
            ).AsContainer<std::vector<GetRecipeIngredientsRow>>(
                userver::storages::postgres::kRowTag
            );
        }

        transaction.Commit();
    } catch (const IntegrityConstraintViolation& error) {
        if (const auto ingredient_error = IngredientErrorMessage(error)) {
            throw ClientError(
                ServiceErrorCode{ingredient_error->code},
                ExternalBody{ingredient_error->message}
            );
        }
        throw;
    }

    if (!body.ingredients) {
        const std::vector<std::int64_t> recipe_ids{id};
        ingredients = LoadIngredients(pg_client_, ClusterHostType::kMaster, recipe_ids);
    }

    return RecipeToJson(*updated, std::span<const GetRecipeIngredientsRow>{ingredients});
}

}  // namespace foodgram::handlers::recipes
