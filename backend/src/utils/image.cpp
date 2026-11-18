#include "utils/image.hpp"

#include <algorithm>
#include <optional>
#include <string>
#include <string_view>

#include <userver/crypto/base64.hpp>
#include <userver/server/handlers/exceptions.hpp>

#include "config.hpp"
#include "handlers/error_codes.hpp"

namespace foodgram::utils {

using userver::server::handlers::ClientError;
using userver::server::handlers::ExternalBody;
using userver::server::handlers::ServiceErrorCode;

namespace {

struct ImageFormat {
    std::string_view content_type;
    std::string_view extension;
};

std::optional<ImageFormat> DetectFormat(std::string_view data) {
    if (data.starts_with(std::string_view{"\xFF\xD8\xFF", 3})) {
        return ImageFormat{"image/jpeg", "jpg"};
    }
    if (data.starts_with(std::string_view{"\x89PNG\r\n\x1A\n", 8})) {
        return ImageFormat{"image/png", "png"};
    }
    if (data.starts_with("GIF87a") || data.starts_with("GIF89a")) {
        return ImageFormat{"image/gif", "gif"};
    }
    if (data.size() >= 12 && data.starts_with("RIFF") && data.substr(8, 4) == "WEBP") {
        return ImageFormat{"image/webp", "webp"};
    }
    return std::nullopt;
}

bool IsAllowed(std::string_view extension, const config::MediaConfig& settings) {
    const auto& allowed = settings.allowed_image_extensions;
    return std::find(allowed.begin(), allowed.end(), extension) != allowed.end();
}

[[noreturn]] void ThrowInvalidImage() {
    throw ClientError(
        ServiceErrorCode{foodgram::handlers::codes::kInvalidImage}, ExternalBody{"Invalid image"}
    );
}

}  // namespace

Image ParseImage(std::string_view value, const userver::dynamic_config::Snapshot& snapshot) {
    std::string_view payload = value;
    if (value.starts_with("data:")) {
        const auto marker = value.find(";base64,");
        if (marker != std::string_view::npos) {
            payload = value.substr(marker + 8);
        }
    }

    Image image{std::string{payload}, {}, {}};

    if (!userver::crypto::base64::Base64UniversalDecodeInPlace(image.data) || image.data.empty()) {
        ThrowInvalidImage();
    }

    const auto format = DetectFormat(image.data);
    if (!format) {
        ThrowInvalidImage();
    }

    if (!IsAllowed(format->extension, snapshot[config::kMedia])) {
        throw ClientError(
            ServiceErrorCode{foodgram::handlers::codes::kUnsupportedImageFormat},
            ExternalBody{"Unsupported image format"}
        );
    }

    image.content_type = format->content_type;
    image.extension = format->extension;
    return image;
}

}  // namespace foodgram::utils
