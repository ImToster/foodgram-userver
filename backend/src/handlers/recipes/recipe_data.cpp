#include "handlers/recipes/recipe_data.hpp"

#include <optional>
#include <utility>
#include <vector>

#include <userver/server/handlers/exceptions.hpp>

namespace foodgram::handlers::recipes {

using userver::server::handlers::ExternalBody;
using userver::server::handlers::ResourceNotFound;
using userver::storages::postgres::ClusterHostType;

std::vector<GetRecipeIngredientsRow> LoadIngredients(
    const foodgram::ClusterPgClient& client,
    ClusterHostType host_type,
    std::span<const std::int64_t> recipe_ids
) {
    if (recipe_ids.empty()) {
        return {};
    }

    const std::vector<std::int64_t> ids{recipe_ids.begin(), recipe_ids.end()};
    return client.GetRecipeIngredients(host_type, ids);
}

GetRecipeRow FindRecipe(
    const foodgram::ClusterPgClient& client,
    ClusterHostType host_type,
    std::int64_t recipe_id,
    std::optional<std::int64_t> viewer_id
) {
    auto rows = client.GetRecipe(host_type, recipe_id, viewer_id);
    if (rows.empty()) {
        throw ResourceNotFound(ExternalBody{"Recipe not found"});
    }
    return std::move(rows.front());
}

}  // namespace foodgram::handlers::recipes
