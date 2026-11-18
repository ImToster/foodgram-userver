#pragma once

#include <cstdint>
#include <string>
#include <string_view>

#include <userver/components/component_base.hpp>
#include <userver/s3api/clients/fwd.hpp>
#include <userver/yaml_config/schema.hpp>

namespace foodgram::utils {
struct Image;
}

namespace foodgram::storage {

class Media final : public userver::components::ComponentBase {
public:
    static constexpr std::string_view kName = "media-storage";

    Media(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& context);

    static userver::yaml_config::Schema GetStaticConfigSchema();

    std::string Put(std::string_view key, std::string data, std::string_view content_type) const;

    std::string PutAvatar(std::int64_t user_id, const utils::Image& image) const;

    std::string PutRecipeImage(const utils::Image& image) const;

    std::string Url(std::string_view key) const;

private:
    std::string ObjectPath(std::string_view key) const;

    userver::s3api::ClientPtr client_;
    std::string bucket_;
    std::string public_url_;
    std::string avatars_prefix_;
    std::string recipes_prefix_;
};

}  // namespace foodgram::storage
