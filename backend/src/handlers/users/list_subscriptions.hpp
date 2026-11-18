#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/formats/json/value.hpp>
#include <userver/server/http/http_request.hpp>
#include <userver/server/request/request_context.hpp>
#include <userver/yaml_config/schema.hpp>

#include "handlers/paginated.hpp"

namespace foodgram::handlers::users {

class ListSubscriptions final : public PaginatedHandler<ListSubscriptions> {
    friend class PaginatedHandler<ListSubscriptions>;

public:
    static constexpr std::string_view kName = "handler-list-subscriptions";

    ListSubscriptions(
        const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context
    );

    static userver::yaml_config::Schema GetStaticConfigSchema();

private:
    struct Query {
        std::optional<std::int64_t> user_id;
        std::int64_t recipes_limit;
    };

    Query ParseQuery(
        const userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext& context
    ) const;

    std::int64_t Count(const Query& query) const;

    userver::formats::json::Value Fetch(const Query& query, const Pagination& pagination) const;

    const std::int64_t recipes_limit_;
};

}  // namespace foodgram::handlers::users
