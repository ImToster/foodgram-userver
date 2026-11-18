#include "handlers/auth/logout.hpp"

#include <userver/server/http/http_status.hpp>

namespace foodgram::handlers::auth {

userver::formats::json::Value Logout::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value&,
    userver::server::request::RequestContext&
) const {
    request.SetResponseStatus(userver::server::http::HttpStatus::kNoContent);
    return {};
}

}  // namespace foodgram::handlers::auth
