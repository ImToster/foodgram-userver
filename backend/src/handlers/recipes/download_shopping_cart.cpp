#include "handlers/recipes/download_shopping_cart.hpp"

#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <userver/components/component_context.hpp>
#include <userver/http/content_type.hpp>
#include <userver/server/handlers/exceptions.hpp>
#include <userver/server/handlers/json_error_builder.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

#include "auth/user_id.hpp"
#include "handlers/error_codes.hpp"
#include "utils/pdf.hpp"

namespace foodgram::handlers::recipes {

using userver::server::handlers::ClientError;
using userver::server::handlers::CustomHandlerException;
using userver::server::handlers::ExternalBody;
using userver::server::handlers::FormattedErrorData;
using userver::server::handlers::JsonErrorBuilder;
using userver::server::handlers::ServiceErrorCode;
using userver::server::http::HttpRequest;
using userver::server::request::RequestContext;
using userver::storages::postgres::ClusterHostType;

DownloadShoppingCart::DownloadShoppingCart(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context
)
    : HttpHandlerBase(config, context),
      cluster_(context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster()),
      pg_client_(cluster_),
      font_(utils::LoadPdfFont(config["font_path"].As<std::string>())) {}

userver::yaml_config::Schema DownloadShoppingCart::GetStaticConfigSchema() {
    return userver::yaml_config::MergeSchemas<userver::server::handlers::HttpHandlerBase>(R"(
type: object
description: Shopping cart PDF handler
additionalProperties: false
properties:
    font_path:
        type: string
        description: path to the TrueType font used to render the PDF
)");
}

std::string DownloadShoppingCart::HandleRequestThrow(
    const HttpRequest& request,
    RequestContext& context
) const {
    const auto user_id = *foodgram::auth::GetUserId(context);
    auto rows = pg_client_.GetShoppingCart(ClusterHostType::kMaster, user_id);

    if (rows.empty()) {
        throw ClientError(
            ServiceErrorCode{codes::kShoppingCartEmpty},
            ExternalBody{"Shopping list is empty"}
        );
    }

    std::vector<utils::ShoppingListIngredient> ingredients;
    ingredients.reserve(rows.size());
    for (auto& row : rows) {
        ingredients.push_back({
            std::move(row.name).value_or(""),
            std::move(row.measurement_unit).value_or(""),
            row.amount.value_or(0),
        });
    }

    auto& response = request.GetHttpResponse();
    response.SetContentType(userver::http::ContentType{"application/pdf"});
    response.SetHeader(
        std::string_view{"Content-Disposition"},
        std::string{"attachment; filename=\"shopping_cart.pdf\""}
    );

    return utils::RenderShoppingListPdf(ingredients, font_);
}

FormattedErrorData DownloadShoppingCart::GetFormattedExternalErrorBody(const CustomHandlerException& exc) const {
    return {JsonErrorBuilder(exc).GetExternalBody(), JsonErrorBuilder::GetContentType()};
}

}  // namespace foodgram::handlers::recipes
