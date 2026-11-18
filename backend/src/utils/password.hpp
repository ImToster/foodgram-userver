#pragma once

#include <string>
#include <string_view>

namespace foodgram::utils {

std::string HashPassword(std::string_view password);

bool CheckPassword(std::string_view password, std::string_view encoded);

}  // namespace foodgram::utils
