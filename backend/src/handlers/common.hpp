#pragma once

#include <foodgram/pg_cluster.hpp>
#include <userver/dynamic_config/snapshot.hpp>
#include <userver/dynamic_config/source.hpp>
#include <userver/formats/json/value.hpp>
#include <userver/server/handlers/exceptions.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/storages/postgres/cluster.hpp>

#include "handlers/error_codes.hpp"
#include "storage/media.hpp"

namespace foodgram::handlers {

class Common : public userver::server::handlers::HttpHandlerJsonBase {
public:
    Common(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& context);

protected:
    template <typename Schema>
    Schema ParseBody(const userver::formats::json::Value& request_json) const {
        try {
            return request_json.As<Schema>();
        } catch (const userver::formats::json::Exception& error) {
            throw userver::server::handlers::ClientError(
                userver::server::handlers::ServiceErrorCode{codes::kValidationError},
                userver::server::handlers::ExternalBody{error.what()}
            );
        }
    }

    userver::storages::postgres::ClusterPtr cluster_;
    const foodgram::ClusterPgClient pg_client_;
    userver::dynamic_config::Source config_source_;
    const storage::Media& media_;
};

}  // namespace foodgram::handlers
