#include "handlers/users/list_users.hpp"

#include <userver/formats/common/type.hpp>
#include <userver/formats/json/value_builder.hpp>

#include "auth/user_id.hpp"
#include "handlers/users/user_json.hpp"

namespace foodgram::handlers::users {

using userver::server::http::HttpRequest;
using userver::server::request::RequestContext;
using userver::storages::postgres::ClusterHostType;
namespace json = userver::formats::json;

ListUsers::Query ListUsers::ParseQuery(const HttpRequest&, RequestContext& context) const {
    return {.viewer_id = foodgram::auth::GetUserId(context)};
}

std::int64_t ListUsers::Count(const Query&) const {
    return pg_client_.CountUsers(ClusterHostType::kSlave).front().value();
}

json::Value ListUsers::Fetch(const Query& query, const Pagination& pagination) const {
    const auto rows = pg_client_.ListUsers(
        ClusterHostType::kSlave, query.viewer_id, pagination.limit, pagination.Offset()
    );

    json::ValueBuilder result{userver::formats::common::Type::kArray};
    for (const auto& row : rows) {
        result.PushBack(UserToJson(row));
    }
    return result.ExtractValue();
}

}  // namespace foodgram::handlers::users
