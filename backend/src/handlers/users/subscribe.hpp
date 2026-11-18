#pragma once

#include <cstdint>
#include <string_view>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/formats/json/value.hpp>
#include <userver/yaml_config/schema.hpp>

#include "handlers/common.hpp"

namespace foodgram::handlers::users {

class Subscribe final : public Common {
public:
    static constexpr std::string_view kName = "handler-subscribe";

    Subscribe(
        const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context
    );

    static userver::yaml_config::Schema GetStaticConfigSchema();

    userver::formats::json::Value HandleRequestJsonThrow(
        const userver::server::http::HttpRequest& request,
        const userver::formats::json::Value& request_json,
        userver::server::request::RequestContext& context
    ) const override;

private:
    const std::int64_t recipes_limit_;
};

}  // namespace foodgram::handlers::users
