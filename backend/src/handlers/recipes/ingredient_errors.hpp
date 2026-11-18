#pragma once

#include <optional>
#include <string>

#include <userver/storages/postgres/exceptions.hpp>

namespace foodgram::handlers::recipes {

struct IngredientError {
    std::string code;
    std::string message;
};

std::optional<IngredientError> IngredientErrorMessage(
    const userver::storages::postgres::IntegrityConstraintViolation& error
);

}  // namespace foodgram::handlers::recipes
