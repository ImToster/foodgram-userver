#include "handlers/auth/login.hpp"

#include <docs/api/openapi-schema.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/server/handlers/exceptions.hpp>

#include "config.hpp"
#include "handlers/error_codes.hpp"
#include "utils/jwt.hpp"
#include "utils/password.hpp"

namespace foodgram::handlers::auth {

using userver::server::handlers::ExternalBody;
using userver::server::handlers::ServiceErrorCode;
using userver::server::handlers::Unauthorized;
using userver::server::http::HttpRequest;
using userver::server::request::RequestContext;
using userver::storages::postgres::ClusterHostType;
namespace json = userver::formats::json;

json::Value Login::HandleRequestJsonThrow(
    const HttpRequest&,
    const json::Value& request_json,
    RequestContext&
) const {
    const auto body = ParseBody<schemas::TokenCreate>(request_json);

    const auto users = pg_client_.GetUserByEmail(ClusterHostType::kMaster, body.email);
    if (users.empty() || !utils::CheckPassword(body.password, users.front().password.value_or(""))) {
        throw Unauthorized(
            ServiceErrorCode{codes::kInvalidCredentials},
            ExternalBody{"Invalid email or password"}
        );
    }

    const auto config = config_source_.GetSnapshot();

    json::ValueBuilder result;
    result["auth_token"] = utils::jwt::GenerateToken(config[config::kJwtSecret], users.front().id.value());
    return result.ExtractValue();
}

}  // namespace foodgram::handlers::auth
