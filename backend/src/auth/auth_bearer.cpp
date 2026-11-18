#include "auth/auth_bearer.hpp"

#include <cstdint>
#include <exception>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include <userver/components/component_context.hpp>
#include <userver/dynamic_config/storage/component.hpp>
#include <userver/http/common_headers.hpp>
#include <userver/server/handlers/exceptions.hpp>

#include "auth/user_id.hpp"
#include "config.hpp"
#include "handlers/error_codes.hpp"
#include "utils/jwt.hpp"

namespace foodgram::auth {

namespace {

void ThrowUnauthorized(
    std::string external_message,
    std::string internal_message = {}
) {
    throw userver::server::handlers::Unauthorized(
        userver::server::handlers::ServiceErrorCode{handlers::codes::kUnauthorized},
        userver::server::handlers::ExternalBody{std::move(external_message)},
        userver::server::handlers::InternalMessage{std::move(internal_message)}
    );
}

class AuthCheckerBearer final : public userver::server::handlers::auth::AuthCheckerBase {
public:
    using AuthCheckResult = userver::server::handlers::auth::AuthCheckResult;

    AuthCheckerBearer(userver::dynamic_config::Source config_source, bool is_required)
        : config_source_(config_source),
          is_required_(is_required) {}

    [[nodiscard]] AuthCheckResult CheckAuth(
        const userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext& request_context
    ) const override;

    [[nodiscard]] bool SupportsUserAuth() const noexcept override { return true; }

private:
    userver::dynamic_config::Source config_source_;
    const bool is_required_;
};

AuthCheckerBearer::AuthCheckResult AuthCheckerBearer::CheckAuth(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& request_context
) const {
    const auto& auth_value = request.GetHeader(userver::http::headers::kAuthorization);

    if (auth_value.empty()) {
        if (is_required_) {
            ThrowUnauthorized("Empty 'Authorization' header");
        }

        SetUserId(request_context, std::nullopt);
        return {};
    }

    constexpr std::string_view kBearerPrefix = "Bearer ";
    if (!auth_value.starts_with(kBearerPrefix)) {
        ThrowUnauthorized("'Authorization' header should have 'Bearer <token>' format");
    }

    const std::string_view token{auth_value.data() + kBearerPrefix.size()};

    const auto secret = config_source_.GetCopy(config::kJwtSecret);

    std::optional<std::int64_t> user_id;
    try {
        user_id = utils::jwt::DecodeToken(secret, token).get_claim_value<std::int64_t>("user_id");
    } catch (const std::exception& error) {
        ThrowUnauthorized("Token verification error", error.what());
    }

    SetUserId(request_context, user_id);
    return {};
}

}  // namespace

CheckerFactory::CheckerFactory(const userver::components::ComponentContext& context)
    : config_source_(context.FindComponent<userver::components::DynamicConfig>().GetSource()) {}

userver::server::handlers::auth::AuthCheckerBasePtr CheckerFactory::MakeAuthChecker(
    const userver::server::handlers::auth::HandlerAuthConfig& auth_config
) const {
    return std::make_shared<AuthCheckerBearer>(config_source_, auth_config["required"].As<bool>(false));
}

}  // namespace foodgram::auth
