#include "handlers/recipes/delete_recipe.hpp"

#include <userver/server/handlers/exceptions.hpp>
#include <userver/server/http/http_status.hpp>

#include "auth/user_id.hpp"
#include "handlers/recipes/recipe_data.hpp"
#include "utils/request.hpp"

namespace foodgram::handlers::recipes {

using userver::server::handlers::CustomHandlerException;
using userver::server::handlers::ExternalBody;
using userver::server::http::HttpRequest;
using userver::server::http::HttpStatus;
using userver::server::request::RequestContext;
using userver::storages::postgres::ClusterHostType;
namespace json = userver::formats::json;

json::Value DeleteRecipe::HandleRequestJsonThrow(
    const HttpRequest& request,
    const json::Value&,
    RequestContext& context
) const {
    const auto id = utils::ParsePathId(request.GetPathArg("id"));
    const auto user_id = *foodgram::auth::GetUserId(context);

    const auto recipe = FindRecipe(pg_client_, ClusterHostType::kMaster, id, user_id);
    if (recipe.author_id.value() != user_id) {
        throw CustomHandlerException(
            HandlerErrorCode::kForbidden, ExternalBody{"Only the author can delete the recipe"}
        );
    }

    pg_client_.DeleteRecipe(ClusterHostType::kMaster, id);

    request.SetResponseStatus(HttpStatus::kNoContent);
    return {};
}

}  // namespace foodgram::handlers::recipes
