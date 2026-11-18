#pragma once

#include <string>
#include <string_view>

#include <userver/dynamic_config/snapshot.hpp>

namespace foodgram::utils {

struct Image {
    std::string data;
    std::string content_type;
    std::string extension;
};

Image ParseImage(std::string_view value, const userver::dynamic_config::Snapshot& snapshot);

}  // namespace foodgram::utils
