#include "handlers/ingredients/list_ingredients.hpp"

#include <optional>
#include <string>

#include <userver/formats/common/type.hpp>
#include <userver/formats/json/value_builder.hpp>

#include "handlers/ingredients/ingredient_json.hpp"

namespace foodgram::handlers::ingredients {

using userver::server::http::HttpRequest;
using userver::server::request::RequestContext;
using userver::storages::postgres::ClusterHostType;
namespace json = userver::formats::json;

json::Value ListIngredients::HandleRequestJsonThrow(
    const HttpRequest& request,
    const json::Value&,
    RequestContext&
) const {
    std::optional<std::string> name;
    if (request.HasArg("name") && !request.GetArg("name").empty()) {
        name = request.GetArg("name");
    }

    const auto rows = pg_client_.ListIngredients(ClusterHostType::kSlave, name);

    json::ValueBuilder result{userver::formats::common::Type::kArray};
    for (const auto& row : rows) {
        result.PushBack(IngredientToJson(row));
    }
    return result.ExtractValue();
}

}  // namespace foodgram::handlers::ingredients
