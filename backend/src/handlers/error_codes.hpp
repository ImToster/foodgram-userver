#pragma once

namespace foodgram::handlers::codes {

inline constexpr const char* kValidationError = "validation_error";
inline constexpr const char* kInvalidCredentials = "invalid_credentials";
inline constexpr const char* kUnauthorized = "unauthorized";
inline constexpr const char* kEmailTaken = "email_taken";
inline constexpr const char* kUsernameTaken = "username_taken";
inline constexpr const char* kUserExists = "user_exists";
inline constexpr const char* kInvalidCurrentPassword = "invalid_current_password";
inline constexpr const char* kCannotSubscribeToSelf = "cannot_subscribe_to_self";
inline constexpr const char* kSubscriptionExists = "subscription_exists";
inline constexpr const char* kCannotUnsubscribeFromSelf = "cannot_unsubscribe_from_self";
inline constexpr const char* kSubscriptionNotExists = "subscription_not_exists";
inline constexpr const char* kRecipeAlreadyFavorited = "recipe_already_favorited";
inline constexpr const char* kRecipeNotFavorited = "recipe_not_favorited";
inline constexpr const char* kRecipeAlreadyInCart = "recipe_already_in_cart";
inline constexpr const char* kRecipeNotInCart = "recipe_not_in_cart";
inline constexpr const char* kShoppingCartEmpty = "shopping_cart_empty";
inline constexpr const char* kIngredientNotFound = "ingredient_not_found";
inline constexpr const char* kDuplicateIngredients = "duplicate_ingredients";
inline constexpr const char* kInvalidImage = "invalid_image";
inline constexpr const char* kUnsupportedImageFormat = "unsupported_image_format";

}  // namespace foodgram::handlers::codes
