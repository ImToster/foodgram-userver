#include "handlers/recipes/ingredient_errors.hpp"

#include <optional>

#include "handlers/error_codes.hpp"

namespace foodgram::handlers::recipes {

using userver::storages::postgres::IntegrityConstraintViolation;

std::optional<IngredientError> IngredientErrorMessage(const IntegrityConstraintViolation& error) {
    const auto constraint = error.GetConstraint();
    if (constraint == "fk_recipe_ingredients_ingredient") {
        return IngredientError{codes::kIngredientNotFound, "Ingredient not found"};
    }
    if (constraint == "uniq_recipe_ingredients_recipe_ingredient") {
        return IngredientError{
            codes::kDuplicateIngredients, "Ingredients cannot contain duplicates"
        };
    }
    return std::nullopt;
}

}  // namespace foodgram::handlers::recipes
