#include "storage/media.hpp"

#include <chrono>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include <fmt/format.h>
#include <userver/clients/http/component.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/s3api/authenticators/access_key.hpp>
#include <userver/s3api/clients/s3api.hpp>
#include <userver/s3api/models/s3api_connection_type.hpp>
#include <userver/s3api/models/secret.hpp>
#include <userver/utils/uuid4.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

#include "utils/image.hpp"

namespace foodgram::storage {

Media::Media(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& context)
    : ComponentBase(config, context),
      bucket_(config["bucket"].As<std::string>()),
      public_url_(config["public-url"].As<std::string>()),
      avatars_prefix_(config["prefixes"]["avatars"].As<std::string>()),
      recipes_prefix_(config["prefixes"]["recipes"].As<std::string>()) {
    auto& http_client = context.FindComponent<userver::components::HttpClient>().GetHttpClient();

    const auto url = config["url"].As<std::string>();
    const auto connection_type =
        url.starts_with("https://") ? userver::s3api::S3ConnectionType::kHttps : userver::s3api::S3ConnectionType::kHttp;

    auto connection = userver::s3api::MakeS3Connection(
        http_client,
        connection_type,
        url,
        userver::s3api::ConnectionCfg{
            std::chrono::milliseconds{config["timeout-ms"].As<int>()},
            config["retries"].As<int>(),
        }
    );

    auto authenticator = std::make_shared<userver::s3api::authenticators::AccessKey>(
        config["access-key"].As<std::string>(),
        userver::s3api::Secret{config["secret-key"].As<std::string>()}
    );

    client_ = userver::s3api::GetS3Client(std::move(connection), std::move(authenticator), {});
}

userver::yaml_config::Schema Media::GetStaticConfigSchema() {
    return userver::yaml_config::MergeSchemas<userver::components::ComponentBase>(R"(
type: object
description: S3 media storage
additionalProperties: false
properties:
    url:
        type: string
        description: S3 endpoint
    public-url:
        type: string
        description: public S3 endpoint used to build object URLs
    bucket:
        type: string
        description: bucket name
    access-key:
        type: string
        description: S3 access key
    secret-key:
        type: string
        description: S3 secret key
    timeout-ms:
        type: integer
        description: S3 request timeout in milliseconds
        default: 1000
    retries:
        type: integer
        description: number of S3 request retries
        default: 2
    prefixes:
        type: object
        description: object key prefixes
        additionalProperties: false
        properties:
            avatars:
                type: string
                description: prefix for user avatars (without trailing slash)
            recipes:
                type: string
                description: prefix for recipe images (without trailing slash)
)");
}

std::string Media::ObjectPath(std::string_view key) const { return fmt::format("{}/{}", bucket_, key); }

std::string Media::Put(std::string_view key, std::string data, std::string_view content_type) const {
    client_->PutObject(ObjectPath(key), std::move(data), std::nullopt, content_type);
    return Url(key);
}

std::string Media::PutAvatar(std::int64_t user_id, const utils::Image& image) const {
    return Put(
        fmt::format("{}/{}.{}", avatars_prefix_, user_id, image.extension),
        image.data,
        image.content_type
    );
}

std::string Media::PutRecipeImage(const utils::Image& image) const {
    return Put(
        fmt::format("{}/{}.{}", recipes_prefix_, userver::utils::generators::GenerateUuid(), image.extension),
        image.data,
        image.content_type
    );
}

std::string Media::Url(std::string_view key) const { return fmt::format("{}/{}", public_url_, key); }

}  // namespace foodgram::storage
