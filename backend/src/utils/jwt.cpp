#include "utils/jwt.hpp"

#include <chrono>

namespace foodgram::utils::jwt {

namespace {

constexpr std::chrono::hours kTokenLifetime{24 * 7};

using namespace ::jwt::params;

}  // namespace

std::string GenerateToken(std::string_view secret_key, std::int64_t user_id) {
    ::jwt::jwt_object token{algorithm("HS256"), secret(secret_key)};
    token.add_claim("user_id", user_id);
    token.add_claim("exp", std::chrono::system_clock::now() + kTokenLifetime);
    return token.signature();
}

::jwt::jwt_payload DecodeToken(std::string_view secret_key, std::string_view token) {
    return ::jwt::decode(token, algorithms({"HS256"}), secret(secret_key), verify(true)).payload();
}

}  // namespace foodgram::utils::jwt
