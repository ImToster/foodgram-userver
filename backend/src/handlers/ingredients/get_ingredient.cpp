#include "handlers/ingredients/get_ingredient.hpp"

#include <userver/server/handlers/exceptions.hpp>

#include "handlers/ingredients/ingredient_json.hpp"
#include "utils/request.hpp"

namespace foodgram::handlers::ingredients {

using userver::server::handlers::ExternalBody;
using userver::server::handlers::ResourceNotFound;
using userver::server::http::HttpRequest;
using userver::server::request::RequestContext;
using userver::storages::postgres::ClusterHostType;
namespace json = userver::formats::json;

json::Value GetIngredient::HandleRequestJsonThrow(
    const HttpRequest& request,
    const json::Value&,
    RequestContext&
) const {
    const auto id = utils::ParsePathId(request.GetPathArg("id"));

    const auto rows = pg_client_.GetIngredient(ClusterHostType::kSlave, id);
    if (rows.empty()) {
        throw ResourceNotFound(ExternalBody{"Ingredient not found"});
    }

    return IngredientToJson(rows.front());
}

}  // namespace foodgram::handlers::ingredients
