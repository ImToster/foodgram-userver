#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

#include <userver/formats/json/value.hpp>
#include <userver/server/http/http_request.hpp>
#include <userver/server/request/request_context.hpp>

#include "handlers/paginated.hpp"

namespace foodgram::handlers::users {

class ListUsers final : public PaginatedHandler<ListUsers> {
    friend class PaginatedHandler<ListUsers>;

public:
    static constexpr std::string_view kName = "handler-list-users";

    using PaginatedHandler::PaginatedHandler;

private:
    struct Query {
        std::optional<std::int64_t> viewer_id;
    };

    Query ParseQuery(
        const userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext& context
    ) const;

    std::int64_t Count(const Query& query) const;

    userver::formats::json::Value Fetch(const Query& query, const Pagination& pagination) const;
};

}  // namespace foodgram::handlers::users
