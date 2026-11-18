#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include <userver/server/request/request_context.hpp>

namespace foodgram::auth {

inline constexpr std::string_view kUserId = "user_id";

inline std::optional<std::int64_t> GetUserId(const userver::server::request::RequestContext& context) {
    return context.GetData<std::optional<std::int64_t>>(kUserId);
}

inline void SetUserId(userver::server::request::RequestContext& context, std::optional<std::int64_t> user_id) {
    context.SetData(std::string{kUserId}, std::move(user_id));
}

}  // namespace foodgram::auth
