#pragma once

#include <cstdint>
#include <string>
#include <string_view>

#include <jwt/jwt.hpp>

namespace foodgram::utils::jwt {

std::string GenerateToken(std::string_view secret, std::int64_t user_id);

::jwt::jwt_payload DecodeToken(std::string_view secret, std::string_view token);

}  // namespace foodgram::utils::jwt
