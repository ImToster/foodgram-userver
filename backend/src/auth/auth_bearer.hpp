#pragma once

#include <string_view>

#include <userver/dynamic_config/source.hpp>
#include <userver/server/handlers/auth/auth_checker_factory.hpp>

namespace foodgram::auth {

class CheckerFactory final : public userver::server::handlers::auth::AuthCheckerFactoryBase {
public:
    static constexpr std::string_view kAuthType = "bearer";

    explicit CheckerFactory(const userver::components::ComponentContext& context);

    userver::server::handlers::auth::AuthCheckerBasePtr MakeAuthChecker(
        const userver::server::handlers::auth::HandlerAuthConfig& auth_config
    ) const override;

private:
    userver::dynamic_config::Source config_source_;
};

}  // namespace foodgram::auth
