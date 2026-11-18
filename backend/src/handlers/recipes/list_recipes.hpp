#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

#include <userver/formats/json/value.hpp>
#include <userver/server/http/http_request.hpp>
#include <userver/server/request/request_context.hpp>

#include "handlers/paginated.hpp"

namespace foodgram::handlers::recipes {

class ListRecipes final : public PaginatedHandler<ListRecipes> {
    friend class PaginatedHandler<ListRecipes>;

public:
    static constexpr std::string_view kName = "handler-list-recipes";

    using PaginatedHandler::PaginatedHandler;

private:
    struct Query {
        std::optional<std::int64_t> viewer_id;
        std::optional<std::int64_t> author_id;
        std::optional<bool> is_favorited;
        std::optional<bool> is_in_shopping_cart;
    };

    Query ParseQuery(
        const userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext& context
    ) const;

    std::int64_t Count(const Query& query) const;

    userver::formats::json::Value Fetch(const Query& query, const Pagination& pagination) const;
};

}  // namespace foodgram::handlers::recipes
