#include "utils/password.hpp"

#include <cstddef>
#include <string>
#include <string_view>

#include <fmt/format.h>
#include <userver/crypto/hash.hpp>
#include <userver/utils/rand.hpp>

namespace foodgram::utils {

namespace {

constexpr std::string_view kAlgorithm = "sha256";
constexpr std::size_t kSaltLength = 32;
constexpr std::string_view kSaltAlphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

std::string GenerateSalt() {
    std::string salt(kSaltLength, '0');
    for (auto& symbol : salt) {
        symbol = kSaltAlphabet[userver::utils::RandRange(kSaltAlphabet.size())];
    }
    return salt;
}

}  // namespace

std::string HashPassword(std::string_view password) {
    const auto salt = GenerateSalt();
    return fmt::format("{}${}${}", kAlgorithm, salt, userver::crypto::hash::Sha256({password, salt}));
}

bool CheckPassword(std::string_view password, std::string_view encoded) {
    const auto algorithm_end = encoded.find('$');
    if (algorithm_end == std::string_view::npos) {
        return false;
    }

    const auto salt_end = encoded.find('$', algorithm_end + 1);
    if (salt_end == std::string_view::npos) {
        return false;
    }

    if (encoded.substr(0, algorithm_end) != kAlgorithm) {
        return false;
    }

    const auto salt = encoded.substr(algorithm_end + 1, salt_end - algorithm_end - 1);
    const auto expected = encoded.substr(salt_end + 1);
    return userver::crypto::hash::Sha256({password, salt}) == expected;
}

}  // namespace foodgram::utils
