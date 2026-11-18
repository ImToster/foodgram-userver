#pragma once

#include <string_view>

#include <userver/formats/json/value.hpp>

#include "handlers/common.hpp"

namespace foodgram::handlers::ingredients {

class ListIngredients final : public Common {
public:
    static constexpr std::string_view kName = "handler-list-ingredients";

    using Common::Common;

    userver::formats::json::Value HandleRequestJsonThrow(
        const userver::server::http::HttpRequest& request,
        const userver::formats::json::Value& request_json,
        userver::server::request::RequestContext& context
    ) const override;
};

}  // namespace foodgram::handlers::ingredients
