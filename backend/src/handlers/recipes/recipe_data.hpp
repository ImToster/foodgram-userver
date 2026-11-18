#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include <vector>

#include <foodgram/pg_cluster.hpp>
#include <userver/storages/postgres/cluster_types.hpp>

namespace foodgram::handlers::recipes {

std::vector<GetRecipeIngredientsRow> LoadIngredients(
    const foodgram::ClusterPgClient& client,
    userver::storages::postgres::ClusterHostType host_type,
    std::span<const std::int64_t> recipe_ids
);

GetRecipeRow FindRecipe(
    const foodgram::ClusterPgClient& client,
    userver::storages::postgres::ClusterHostType host_type,
    std::int64_t recipe_id,
    std::optional<std::int64_t> viewer_id
);

}  // namespace foodgram::handlers::recipes
