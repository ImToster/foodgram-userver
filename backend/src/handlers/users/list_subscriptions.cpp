#include "handlers/users/list_subscriptions.hpp"

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

#include <userver/formats/common/type.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

#include "auth/user_id.hpp"
#include "handlers/users/user_json.hpp"
#include "utils/request.hpp"

namespace foodgram::handlers::users {

using userver::server::http::HttpRequest;
using userver::server::request::RequestContext;
using userver::storages::postgres::ClusterHostType;
namespace json = userver::formats::json;

ListSubscriptions::ListSubscriptions(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context
)
    : PaginatedHandler(config, context),
      recipes_limit_(config["recipes_limit"].As<std::int64_t>()) {}

userver::yaml_config::Schema ListSubscriptions::GetStaticConfigSchema() {
    return userver::yaml_config::MergeSchemas<PaginatedHandler<ListSubscriptions>>(R"(
type: object
description: Subscriptions list handler
additionalProperties: false
properties:
    recipes_limit:
        type: integer
        description: maximum number of recipes embedded into each user
)");
}

ListSubscriptions::Query ListSubscriptions::ParseQuery(const HttpRequest& request, RequestContext& context) const {
    return {
        .user_id = foodgram::auth::GetUserId(context),
        .recipes_limit = utils::ParseRecipesLimit(request, recipes_limit_),
    };
}

std::int64_t ListSubscriptions::Count(const Query& query) const {
    return pg_client_.CountSubscriptions(ClusterHostType::kSlave, query.user_id).front().value();
}

json::Value ListSubscriptions::Fetch(const Query& query, const Pagination& pagination) const {
    const auto rows = pg_client_.ListSubscriptions(
        ClusterHostType::kSlave, query.user_id, pagination.limit, pagination.Offset()
    );

    std::vector<std::int64_t> author_ids;
    author_ids.reserve(rows.size());
    for (const auto& row : rows) {
        author_ids.push_back(row.id.value());
    }

    std::vector<ListSubscriptionRecipesRow> recipe_rows;
    if (!author_ids.empty()) {
        recipe_rows = pg_client_.ListSubscriptionRecipes(
            ClusterHostType::kSlave, author_ids, query.recipes_limit
        );
    }

    json::ValueBuilder result{userver::formats::common::Type::kArray};

    const std::span<const ListSubscriptionRecipesRow> recipe_span{recipe_rows};
    std::size_t begin = 0;

    for (const auto& row : rows) {
        std::size_t end = begin;
        while (end < recipe_span.size() && recipe_span[end].author_id.value() == row.id.value()) {
            ++end;
        }

        result.PushBack(UserWithRecipesToJson(row, recipe_span.subspan(begin, end - begin)));
        begin = end;
    }

    return result.ExtractValue();
}

}  // namespace foodgram::handlers::users
