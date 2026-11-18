#pragma once

#include <string>
#include <vector>

#include <userver/dynamic_config/snapshot.hpp>
#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/parse/to.hpp>

namespace foodgram::config {

struct MediaConfig {
    std::vector<std::string> allowed_image_extensions;
};

inline MediaConfig Parse(const userver::formats::json::Value& value, userver::formats::parse::To<MediaConfig>) {
    return MediaConfig{
        .allowed_image_extensions = value["allowed_image_extensions"].As<std::vector<std::string>>(),
    };
}

inline const userver::dynamic_config::Key<std::string> kJwtSecret{"JWT_SECRET", "foodgram-secret"};

inline const userver::dynamic_config::Key<MediaConfig> kMedia{
    "MEDIA",
    userver::dynamic_config::DefaultAsJsonString{"{}"},
};

}  // namespace foodgram::config
