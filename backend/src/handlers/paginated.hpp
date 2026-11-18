#pragma once

#include <algorithm>
#include <cstdint>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/formats/json/value.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/server/handlers/exceptions.hpp>
#include <userver/server/http/http_request.hpp>
#include <userver/server/request/request_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/yaml_config/schema.hpp>

#include "handlers/common.hpp"
#include "utils/request.hpp"

namespace foodgram::handlers {

struct Pagination {
    std::int64_t page;
    std::int64_t limit;

    std::int64_t Offset() const { return (page - 1) * limit; }
};

template <typename Derived>
class PaginatedHandler : public Common {
public:
    PaginatedHandler(
        const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context
    )
        : Common(config, context),
          default_page_size_(config["pagination"]["default_page_size"].As<std::int64_t>()),
          max_page_size_(config["pagination"]["max_page_size"].As<std::int64_t>()) {}

    static userver::yaml_config::Schema GetStaticConfigSchema() {
        return userver::yaml_config::MergeSchemas<Common>(R"(
type: object
description: Handler with a paginated JSON response
additionalProperties: false
properties:
    pagination:
        type: object
        description: Pagination settings
        additionalProperties: false
        properties:
            default_page_size:
                type: integer
                description: page size used when the limit query argument is absent
            max_page_size:
                type: integer
                description: upper bound for the limit query argument
)");
    }

protected:
    userver::formats::json::Value HandleRequestJsonThrow(
        const userver::server::http::HttpRequest& request,
        const userver::formats::json::Value&,
        userver::server::request::RequestContext& context
    ) const final {
        const auto& self = static_cast<const Derived&>(*this);

        const auto pagination = ParsePagination(request);
        const auto query = self.ParseQuery(request, context);
        const auto count = self.Count(query);
        CheckPageInRange(pagination, count);

        userver::formats::json::ValueBuilder result;
        result["count"] = count;
        result["results"] = self.Fetch(query, pagination);
        return result.ExtractValue();
    }

private:
    Pagination ParsePagination(const userver::server::http::HttpRequest& request) const {
        Pagination pagination{1, default_page_size_};

        if (request.HasArg("page")) {
            const auto page = utils::TryParseIntegerArg(request, "page");
            if (!page) {
                throw userver::server::handlers::ResourceNotFound(
                    userver::server::handlers::ExternalBody{"Page not found"}
                );
            }
            pagination.page = *page;
        }

        if (const auto limit = utils::TryParseIntegerArg(request, "limit"); limit && *limit > 0) {
            pagination.limit = std::min(*limit, max_page_size_);
        }

        return pagination;
    }

    static void CheckPageInRange(const Pagination& pagination, std::int64_t count) {
        const auto pages = count > 0 ? (count + pagination.limit - 1) / pagination.limit : 1;
        if (pagination.page < 1 || pagination.page > pages) {
            throw userver::server::handlers::ResourceNotFound(
                userver::server::handlers::ExternalBody{"Page not found"}
            );
        }
    }

    const std::int64_t default_page_size_;
    const std::int64_t max_page_size_;
};

}  // namespace foodgram::handlers
