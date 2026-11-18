#pragma once

#include <cstdint>
#include <exception>
#include <optional>
#include <string>
#include <string_view>

#include <fmt/format.h>
#include <userver/server/handlers/exceptions.hpp>
#include <userver/server/http/http_request.hpp>

namespace foodgram::utils {

inline std::int64_t ParsePathId(std::string_view value) {
    try {
        return std::stoll(std::string{value});
    } catch (const std::exception&) {
        throw userver::server::handlers::ResourceNotFound(
            userver::server::handlers::ExternalBody{"Object not found"}
        );
    }
}

inline std::optional<std::int64_t> TryParseIntegerArg(
    const userver::server::http::HttpRequest& request,
    std::string_view name
) {
    if (!request.HasArg(name)) {
        return std::nullopt;
    }

    const auto& value = request.GetArg(name);
    if (value.empty()) {
        return std::nullopt;
    }

    try {
        return std::stoll(value);
    } catch (const std::exception&) {
        return std::nullopt;
    }
}

inline std::optional<std::int64_t> ParseIntegerArg(
    const userver::server::http::HttpRequest& request,
    std::string_view name
) {
    const auto value = TryParseIntegerArg(request, name);
    if (value || !request.HasArg(name) || request.GetArg(name).empty()) {
        return value;
    }

    throw userver::server::handlers::ClientError(
        userver::server::handlers::ExternalBody{fmt::format("Invalid '{}' query argument", name)}
    );
}

inline std::int64_t ParseRecipesLimit(
    const userver::server::http::HttpRequest& request,
    std::int64_t default_limit
) {
    const auto value = TryParseIntegerArg(request, "recipes_limit");
    return value && *value > 0 ? *value : default_limit;
}

inline std::optional<bool> ParseBooleanArg(
    const userver::server::http::HttpRequest& request,
    std::string_view name
) {
    if (!request.HasArg(name)) {
        return std::nullopt;
    }

    const auto& value = request.GetArg(name);
    if (value.empty()) {
        return std::nullopt;
    }
    if (value == "1" || value == "true") {
        return true;
    }
    if (value == "0" || value == "false") {
        return false;
    }

    throw userver::server::handlers::ClientError(
        userver::server::handlers::ExternalBody{fmt::format("Invalid '{}' query argument", name)}
    );
}

}  // namespace foodgram::utils
