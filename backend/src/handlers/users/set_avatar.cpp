#include "handlers/users/set_avatar.hpp"

#include <string>

#include <docs/api/openapi-schema.hpp>
#include <userver/formats/json/value_builder.hpp>

#include "auth/user_id.hpp"
#include "utils/image.hpp"

namespace foodgram::handlers::users {

using userver::server::http::HttpRequest;
using userver::server::request::RequestContext;
using userver::storages::postgres::ClusterHostType;
namespace json = userver::formats::json;

json::Value SetAvatar::HandleRequestJsonThrow(
    const HttpRequest&,
    const json::Value& request_json,
    RequestContext& context
) const {
    const auto body = ParseBody<schemas::SetAvatar>(request_json);
    const auto image = utils::ParseImage(body.avatar, config_source_.GetSnapshot());

    const auto user_id = foodgram::auth::GetUserId(context);

    const auto url = media_.PutAvatar(*user_id, image);

    pg_client_.UpdateAvatar(ClusterHostType::kMaster, user_id, url);

    json::ValueBuilder result;
    result["avatar"] = url;
    return result.ExtractValue();
}

}  // namespace foodgram::handlers::users
