#pragma once

#include <span>

#include <userver/formats/common/type.hpp>
#include <userver/formats/json/value_builder.hpp>

#include "handlers/recipes/recipe_json.hpp"

namespace foodgram::handlers::users {

template <typename UserRow>
void AddUserFields(userver::formats::json::ValueBuilder& builder, const UserRow& row) {
    builder["id"] = row.id.value();
    builder["email"] = row.email.value();
    builder["username"] = row.username.value();
    builder["first_name"] = row.first_name.value();
    builder["last_name"] = row.last_name.value();
    builder["is_subscribed"] = row.is_subscribed.value();
    if (row.avatar) {
        builder["avatar"] = *row.avatar;
    } else {
        builder["avatar"] = nullptr;
    }
}

template <typename UserRow>
userver::formats::json::Value UserToJson(const UserRow& row) {
    userver::formats::json::ValueBuilder builder;
    AddUserFields(builder, row);
    return builder.ExtractValue();
}

template <typename UserRow>
userver::formats::json::Value CreatedUserToJson(const UserRow& row) {
    userver::formats::json::ValueBuilder builder;
    builder["id"] = row.id.value();
    builder["email"] = row.email.value();
    builder["username"] = row.username.value();
    builder["first_name"] = row.first_name.value();
    builder["last_name"] = row.last_name.value();
    return builder.ExtractValue();
}

template <typename UserRow, typename RecipeRow>
userver::formats::json::Value UserWithRecipesToJson(const UserRow& row, std::span<const RecipeRow> recipes) {
    userver::formats::json::ValueBuilder builder;
    AddUserFields(builder, row);
    builder["recipes"] = userver::formats::json::ValueBuilder{userver::formats::common::Type::kArray};

    for (const auto& recipe : recipes) {
        builder["recipes"].PushBack(recipes::RecipeMinifiedToJson(recipe));
    }

    builder["recipes_count"] = row.recipes_count.value();
    return builder.ExtractValue();
}

}  // namespace foodgram::handlers::users
