#include "handlers/recipes/add_favorite.hpp"

#include <userver/server/handlers/exceptions.hpp>
#include <userver/server/http/http_status.hpp>
#include <userver/storages/postgres/exceptions.hpp>

#include "auth/user_id.hpp"
#include "handlers/error_codes.hpp"
#include "handlers/recipes/recipe_data.hpp"
#include "handlers/recipes/recipe_json.hpp"
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

json::Value AddFavorite::HandleRequestJsonThrow(
    const HttpRequest& request,
    const json::Value&,
    RequestContext& context
) const {
    const auto id = utils::ParsePathId(request.GetPathArg("id"));
    const auto user_id = *foodgram::auth::GetUserId(context);

    const auto recipe = FindRecipe(pg_client_, ClusterHostType::kMaster, id, user_id);

    try {
        pg_client_.AddFavorite(ClusterHostType::kMaster, user_id, id);
    } catch (const userver::storages::postgres::UniqueViolation&) {
        throw ConflictError(
            ServiceErrorCode{codes::kRecipeAlreadyFavorited},
            ExternalBody{"Recipe already is favorited"}
        );
    }

    request.SetResponseStatus(HttpStatus::kCreated);
    return RecipeMinifiedToJson(recipe);
}

}  // namespace foodgram::handlers::recipes
