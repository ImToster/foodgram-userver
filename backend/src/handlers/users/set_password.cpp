#include "handlers/users/set_password.hpp"

#include <docs/api/openapi-schema.hpp>
#include <userver/server/handlers/exceptions.hpp>
#include <userver/server/http/http_status.hpp>

#include "auth/user_id.hpp"
#include "handlers/error_codes.hpp"
#include "utils/password.hpp"

namespace foodgram::handlers::users {

using userver::server::handlers::ClientError;
using userver::server::handlers::ExternalBody;
using userver::server::handlers::ResourceNotFound;
using userver::server::handlers::ServiceErrorCode;
using userver::server::http::HttpRequest;
using userver::server::http::HttpStatus;
using userver::server::request::RequestContext;
using userver::storages::postgres::ClusterHostType;
namespace json = userver::formats::json;

json::Value SetPassword::HandleRequestJsonThrow(
    const HttpRequest& request,
    const json::Value& request_json,
    RequestContext& context
) const {
    const auto body = ParseBody<schemas::SetPassword>(request_json);
    const auto user_id = foodgram::auth::GetUserId(context);

    const auto passwords = pg_client_.GetPassword(ClusterHostType::kMaster, user_id);
    if (passwords.empty() || !passwords.front()) {
        throw ResourceNotFound(ExternalBody{"User not found"});
    }

    if (!utils::CheckPassword(body.current_password, *passwords.front())) {
        throw ClientError(
            ServiceErrorCode{codes::kInvalidCurrentPassword},
            ExternalBody{"Invalid current password"}
        );
    }

    pg_client_.UpdatePassword(ClusterHostType::kMaster, user_id, utils::HashPassword(body.new_password));

    request.SetResponseStatus(HttpStatus::kNoContent);
    return {};
}

}  // namespace foodgram::handlers::users
