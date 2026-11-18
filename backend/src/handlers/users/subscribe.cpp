#include "handlers/users/subscribe.hpp"

#include <cstdint>
#include <span>
#include <vector>

#include <userver/server/handlers/exceptions.hpp>
#include <userver/server/http/http_status.hpp>
#include <userver/storages/postgres/exceptions.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

#include "auth/user_id.hpp"
#include "handlers/error_codes.hpp"
#include "handlers/users/user_json.hpp"
#include "utils/request.hpp"

namespace foodgram::handlers::users {

using userver::server::handlers::ClientError;
using userver::server::handlers::ConflictError;
using userver::server::handlers::ExternalBody;
using userver::server::handlers::ResourceNotFound;
using userver::server::handlers::ServiceErrorCode;
using userver::server::http::HttpRequest;
using userver::server::http::HttpStatus;
using userver::server::request::RequestContext;
using userver::storages::postgres::ClusterHostType;
namespace json = userver::formats::json;

Subscribe::Subscribe(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context
)
    : Common(config, context),
      recipes_limit_(config["recipes_limit"].As<std::int64_t>()) {}

userver::yaml_config::Schema Subscribe::GetStaticConfigSchema() {
    return userver::yaml_config::MergeSchemas<Common>(R"(
type: object
description: Subscribe handler
additionalProperties: false
properties:
    recipes_limit:
        type: integer
        description: maximum number of recipes embedded into the author
)");
}

json::Value Subscribe::HandleRequestJsonThrow(
    const HttpRequest& request,
    const json::Value&,
    RequestContext& context
) const {
    const auto id = utils::ParsePathId(request.GetPathArg("id"));
    const auto user_id = foodgram::auth::GetUserId(context);
    const auto recipes_limit = utils::ParseRecipesLimit(request, recipes_limit_);

    auto authors = pg_client_.GetUserWithRecipesCount(ClusterHostType::kMaster, id, user_id);
    if (authors.empty()) {
        throw ResourceNotFound(ExternalBody{"User not found"});
    }

    if (id == *user_id) {
        throw ClientError(
            ServiceErrorCode{codes::kCannotSubscribeToSelf},
            ExternalBody{"Cannot subscribe to yourself"}
        );
    }

    try {
        pg_client_.Subscribe(ClusterHostType::kMaster, *user_id, id);
    } catch (const userver::storages::postgres::UniqueViolation&) {
        throw ConflictError(
            ServiceErrorCode{codes::kSubscriptionExists},
            ExternalBody{"Subscription already exists"}
        );
    }

    auto& author = authors.front();
    author.is_subscribed = true;

    const auto recipes_rows =
        pg_client_.ListSubscriptionRecipes(ClusterHostType::kMaster, std::vector<std::int64_t>{id}, recipes_limit);

    auto result = UserWithRecipesToJson(author, std::span{recipes_rows});

    request.SetResponseStatus(HttpStatus::kCreated);
    return result;
}

}  // namespace foodgram::handlers::users
