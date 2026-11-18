#include "handlers/recipes/get_recipe.hpp"

#include <cstdint>
#include <span>
#include <vector>

#include "auth/user_id.hpp"
#include "handlers/recipes/recipe_data.hpp"
#include "handlers/recipes/recipe_json.hpp"
#include "utils/request.hpp"

namespace foodgram::handlers::recipes {

using userver::server::http::HttpRequest;
using userver::server::request::RequestContext;
using userver::storages::postgres::ClusterHostType;
namespace json = userver::formats::json;

json::Value GetRecipe::HandleRequestJsonThrow(
    const HttpRequest& request,
    const json::Value&,
    RequestContext& context
) const {
    const auto id = utils::ParsePathId(request.GetPathArg("id"));
    const auto viewer_id = foodgram::auth::GetUserId(context);

    const auto recipe = FindRecipe(pg_client_, ClusterHostType::kSlave, id, viewer_id);
    const std::vector<std::int64_t> recipe_ids{id};
    const auto ingredients = LoadIngredients(pg_client_, ClusterHostType::kSlave, recipe_ids);

    return RecipeToJson(recipe, std::span<const GetRecipeIngredientsRow>{ingredients});
}

}  // namespace foodgram::handlers::recipes
