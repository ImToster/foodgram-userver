#include "handlers/users/register_user.hpp"

#include <vector>

#include <docs/api/openapi-schema.hpp>
#include <userver/server/handlers/exceptions.hpp>
#include <userver/server/http/http_status.hpp>
#include <userver/storages/postgres/exceptions.hpp>

#include "handlers/error_codes.hpp"
#include "handlers/users/user_json.hpp"
#include "utils/password.hpp"

namespace foodgram::handlers::users {

using userver::server::handlers::ConflictError;
using userver::server::handlers::ExternalBody;
using userver::server::handlers::ServiceErrorCode;
using userver::server::http::HttpRequest;
using userver::server::http::HttpStatus;
using userver::server::request::RequestContext;
using userver::storages::postgres::ClusterHostType;
namespace json = userver::formats::json;

json::Value RegisterUser::HandleRequestJsonThrow(
    const HttpRequest& request,
    const json::Value& request_json,
    RequestContext&
) const {
    const auto body = ParseBody<schemas::CustomUserCreate>(request_json);

    const auto password_hash = utils::HashPassword(body.password);

    std::vector<CreateUserRow> rows;
    try {
        rows = pg_client_.CreateUser(
            ClusterHostType::kMaster, body.email, body.username, body.first_name, body.last_name, password_hash
        );
    } catch (const userver::storages::postgres::UniqueViolation& ex) {
        const auto constraint = ex.GetServerMessage().GetConstraint();
        if (constraint == "uniq_users_email") {
            throw ConflictError(
                ServiceErrorCode{codes::kEmailTaken},
                ExternalBody{"User with this email already exists"}
            );
        }
        if (constraint == "uniq_users_username") {
            throw ConflictError(
                ServiceErrorCode{codes::kUsernameTaken},
                ExternalBody{"User with this username already exists"}
            );
        }
        throw ConflictError(
            ServiceErrorCode{codes::kUserExists}, ExternalBody{"User already exists"}
        );
    }

    request.SetResponseStatus(HttpStatus::kCreated);
    return CreatedUserToJson(rows.front());
}

}  // namespace foodgram::handlers::users
