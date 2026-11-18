#include "handlers/users/delete_avatar.hpp"

#include <userver/server/http/http_status.hpp>

#include "auth/user_id.hpp"

namespace foodgram::handlers::users {

using userver::server::http::HttpRequest;
using userver::server::http::HttpStatus;
using userver::server::request::RequestContext;
using userver::storages::postgres::ClusterHostType;
namespace json = userver::formats::json;

json::Value DeleteAvatar::HandleRequestJsonThrow(
    const HttpRequest& request,
    const json::Value&,
    RequestContext& context
) const {
    const auto user_id = foodgram::auth::GetUserId(context);

    pg_client_.DeleteAvatar(ClusterHostType::kMaster, user_id);

    request.SetResponseStatus(HttpStatus::kNoContent);
    return {};
}

}  // namespace foodgram::handlers::users
