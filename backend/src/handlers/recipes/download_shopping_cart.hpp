#pragma once

#include <string>
#include <string_view>

#include <foodgram/pg_cluster.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/formatted_error_data.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/yaml_config/schema.hpp>

#include "utils/pdf.hpp"

namespace foodgram::handlers::recipes {

class DownloadShoppingCart final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-download-shopping-cart";

    DownloadShoppingCart(
        const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context
    );

    static userver::yaml_config::Schema GetStaticConfigSchema();

    std::string HandleRequestThrow(
        const userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext& context
    ) const override;

protected:
    userver::server::handlers::FormattedErrorData GetFormattedExternalErrorBody(
        const userver::server::handlers::CustomHandlerException& exc
    ) const override;

private:
    userver::storages::postgres::ClusterPtr cluster_;
    const foodgram::ClusterPgClient pg_client_;
    const foodgram::utils::PdfFont font_;
};

}  // namespace foodgram::handlers::recipes
