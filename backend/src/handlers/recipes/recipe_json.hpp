#pragma once

#include <span>
#include <utility>

#include <userver/formats/common/type.hpp>
#include <userver/formats/json/value_builder.hpp>

namespace foodgram::handlers::recipes {

template <typename RecipeRow>
void AddRecipeMinifiedFields(userver::formats::json::ValueBuilder& builder, const RecipeRow& row) {
    builder["id"] = row.id.value();
    builder["name"] = row.name.value();
    builder["image"] = row.image.value();
    builder["cooking_time"] = row.cooking_time.value();
}

template <typename RecipeRow>
userver::formats::json::Value RecipeMinifiedToJson(const RecipeRow& row) {
    userver::formats::json::ValueBuilder builder;
    AddRecipeMinifiedFields(builder, row);
    return builder.ExtractValue();
}

template <typename RecipeRow>
void AddAuthorFields(userver::formats::json::ValueBuilder& builder, const RecipeRow& row) {
    builder["id"] = row.author_id.value();
    builder["email"] = row.author_email.value();
    builder["username"] = row.author_username.value();
    builder["first_name"] = row.author_first_name.value();
    builder["last_name"] = row.author_last_name.value();
    builder["is_subscribed"] = row.author_is_subscribed.value();
    if (row.author_avatar) {
        builder["avatar"] = *row.author_avatar;
    } else {
        builder["avatar"] = nullptr;
    }
}

template <typename RecipeRow, typename IngredientRow>
userver::formats::json::Value RecipeToJson(const RecipeRow& row, std::span<const IngredientRow> ingredients) {
    userver::formats::json::ValueBuilder builder;
    builder["id"] = row.id.value();

    userver::formats::json::ValueBuilder author;
    AddAuthorFields(author, row);
    builder["author"] = std::move(author);

    builder["ingredients"] = userver::formats::json::ValueBuilder{userver::formats::common::Type::kArray};
    for (const auto& ingredient : ingredients) {
        userver::formats::json::ValueBuilder item;
        item["id"] = ingredient.id.value();
        item["name"] = ingredient.name.value();
        item["measurement_unit"] = ingredient.measurement_unit.value();
        item["amount"] = ingredient.amount.value();
        builder["ingredients"].PushBack(std::move(item));
    }

    builder["is_favorited"] = row.is_favorited.value();
    builder["is_in_shopping_cart"] = row.is_in_shopping_cart.value();
    builder["name"] = row.name.value();
    builder["image"] = row.image.value();
    builder["text"] = row.text.value();
    builder["cooking_time"] = row.cooking_time.value();
    return builder.ExtractValue();
}

}  // namespace foodgram::handlers::recipes
