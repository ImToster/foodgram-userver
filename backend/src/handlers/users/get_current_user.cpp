#include "handlers/users/get_current_user.hpp"

#include <userver/server/handlers/exceptions.hpp>

#include "auth/user_id.hpp"
#include "handlers/users/user_json.hpp"

namespace foodgram::handlers::users {

using userver::server::handlers::ExternalBody;
using userver::server::handlers::ResourceNotFound;
using userver::server::http::HttpRequest;
using userver::server::request::RequestContext;
using userver::storages::postgres::ClusterHostType;
namespace json = userver::formats::json;

json::Value GetCurrentUser::HandleRequestJsonThrow(
    const HttpRequest&,
    const json::Value&,
    RequestContext& context
) const {
    const auto user_id = foodgram::auth::GetUserId(context);

    const auto rows = pg_client_.GetUser(ClusterHostType::kSlave, user_id, user_id);
    if (rows.empty()) {
        throw ResourceNotFound(ExternalBody{"User not found"});
    }

    return UserToJson(rows.front());
}

}  // namespace foodgram::handlers::users
