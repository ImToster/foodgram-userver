#include "handlers/recipes/list_recipes.hpp"

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

#include <userver/formats/common/type.hpp>
#include <userver/formats/json/value_builder.hpp>

#include "auth/user_id.hpp"
#include "handlers/recipes/recipe_data.hpp"
#include "handlers/recipes/recipe_json.hpp"
#include "utils/request.hpp"

namespace foodgram::handlers::recipes {

using userver::server::http::HttpRequest;
using userver::server::request::RequestContext;
using userver::storages::postgres::ClusterHostType;
namespace json = userver::formats::json;

ListRecipes::Query ListRecipes::ParseQuery(const HttpRequest& request, RequestContext& context) const {
    return {
        .viewer_id = foodgram::auth::GetUserId(context),
        .author_id = utils::ParseIntegerArg(request, "author"),
        .is_favorited = utils::ParseBooleanArg(request, "is_favorited"),
        .is_in_shopping_cart = utils::ParseBooleanArg(request, "is_in_shopping_cart"),
    };
}

std::int64_t ListRecipes::Count(const Query& query) const {
    return pg_client_.CountRecipes(
        ClusterHostType::kSlave,
        query.viewer_id,
        query.author_id,
        query.is_favorited,
        query.is_in_shopping_cart
    ).front().value();
}

json::Value ListRecipes::Fetch(const Query& query, const Pagination& pagination) const {
    const auto rows = pg_client_.ListRecipes(
        ClusterHostType::kSlave,
        query.viewer_id,
        query.author_id,
        query.is_favorited,
        query.is_in_shopping_cart,
        pagination.limit,
        pagination.Offset()
    );

    std::vector<std::int64_t> recipe_ids;
    recipe_ids.reserve(rows.size());
    for (const auto& row : rows) {
        recipe_ids.push_back(row.id.value());
    }

    const auto ingredients = LoadIngredients(pg_client_, ClusterHostType::kSlave, recipe_ids);

    const std::span<const GetRecipeIngredientsRow> ingredient_span{ingredients};
    json::ValueBuilder result{userver::formats::common::Type::kArray};
    std::size_t begin = 0;

    for (const auto& row : rows) {
        std::size_t end = begin;
        while (end < ingredient_span.size() && ingredient_span[end].recipe_id.value() == row.id.value()) {
            ++end;
        }

        result.PushBack(RecipeToJson(row, ingredient_span.subspan(begin, end - begin)));
        begin = end;
    }

    return result.ExtractValue();
}

}  // namespace foodgram::handlers::recipes
