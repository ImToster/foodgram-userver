#include "handlers/recipes/remove_favorite.hpp"

#include <foodgram/sql_queries.hpp>
#include <userver/server/handlers/exceptions.hpp>
#include <userver/server/http/http_status.hpp>

#include "auth/user_id.hpp"
#include "handlers/error_codes.hpp"
#include "handlers/recipes/recipe_data.hpp"
#include "utils/request.hpp"

namespace foodgram::handlers::recipes {

using userver::server::handlers::ConflictError;
using userver::server::handlers::ExternalBody;
using userver::server::handlers::ServiceErrorCode;
using userver::server::http::HttpRequest;
using userver::server::http::HttpStatus;
using userver::server::request::RequestContext;
using userver::storages::postgres::ClusterHostType;
namespace json = userver::formats::json;

json::Value RemoveFavorite::HandleRequestJsonThrow(
    const HttpRequest& request,
    const json::Value&,
    RequestContext& context
) const {
    const auto id = utils::ParsePathId(request.GetPathArg("id"));
    const auto user_id = *foodgram::auth::GetUserId(context);

    FindRecipe(pg_client_, ClusterHostType::kMaster, id, user_id);

    const auto result = cluster_->Execute(ClusterHostType::kMaster, foodgram::sql::kRemoveFavorite, user_id, id);
    if (result.RowsAffected() == 0) {
        throw ConflictError(
            ServiceErrorCode{codes::kRecipeNotFavorited},
            ExternalBody{"Recipe is not favorited"}
        );
    }

    request.SetResponseStatus(HttpStatus::kNoContent);
    return {};
}

}  // namespace foodgram::handlers::recipes
