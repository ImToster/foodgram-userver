#include "handlers/common.hpp"

#include <userver/components/component_context.hpp>
#include <userver/dynamic_config/storage/component.hpp>
#include <userver/storages/postgres/component.hpp>

namespace foodgram::handlers {

Common::Common(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& context)
    : HttpHandlerJsonBase(config, context),
      cluster_(context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster()),
      pg_client_(cluster_),
      config_source_(context.FindComponent<userver::components::DynamicConfig>().GetSource()),
      media_(context.FindComponent<storage::Media>()) {}

}  // namespace foodgram::handlers
