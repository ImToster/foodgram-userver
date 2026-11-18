#pragma once

#include <userver/formats/json/value.hpp>
#include <userver/formats/json/value_builder.hpp>

namespace foodgram::handlers::ingredients {

template <typename IngredientRow>
userver::formats::json::Value IngredientToJson(const IngredientRow& row) {
    userver::formats::json::ValueBuilder builder;
    builder["id"] = row.id.value();
    builder["name"] = row.name.value();
    builder["measurement_unit"] = row.measurement_unit.value();
    return builder.ExtractValue();
}

}  // namespace foodgram::handlers::ingredients
