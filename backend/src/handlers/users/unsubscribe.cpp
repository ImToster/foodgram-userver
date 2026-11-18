#include "handlers/users/unsubscribe.hpp"

#include <foodgram/sql_queries.hpp>
#include <userver/server/handlers/exceptions.hpp>
#include <userver/server/http/http_status.hpp>

#include "auth/user_id.hpp"
#include "handlers/error_codes.hpp"
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

json::Value Unsubscribe::HandleRequestJsonThrow(
    const HttpRequest& request,
    const json::Value&,
    RequestContext& context
) const {
    const auto id = utils::ParsePathId(request.GetPathArg("id"));
    const auto user_id = foodgram::auth::GetUserId(context);

    if (pg_client_.GetUser(ClusterHostType::kMaster, id, user_id).empty()) {
        throw ResourceNotFound(ExternalBody{"User not found"});
    }

    if (id == *user_id) {
        throw ClientError(
            ServiceErrorCode{codes::kCannotUnsubscribeFromSelf},
            ExternalBody{"Cannot unsubscribe from yourself"}
        );
    }

    const auto result = cluster_->Execute(ClusterHostType::kMaster, foodgram::sql::kUnsubscribe, *user_id, id);
    if (result.RowsAffected() == 0) {
        throw ConflictError(
            ServiceErrorCode{codes::kSubscriptionNotExists},
            ExternalBody{"Subscription does not exist"}
        );
    }

    request.SetResponseStatus(HttpStatus::kNoContent);
    return {};
}

}  // namespace foodgram::handlers::users
