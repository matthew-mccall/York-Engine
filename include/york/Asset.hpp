#if !defined(YORK_ASSET_HPP)
#define YORK_ASSET_HPP

#include <filesystem>
#include <memory>
#include <optional>
#include <string>

#include "Async.hpp"

namespace york {

struct Asset {

    enum class Type {
        IMG_PNG,
        SHADER_FRAG_GLSL,
        SHADER_FRAG_SPIRV,
        SHADER_VERT_GLSL,
        SHADER_VERT_SPIRV,
        TOML,
        UTF8,
        RAW,
        AUTO
    };

    std::filesystem::path filepath;
    Type type;

    Asset(const std::string& path, const Type type = Type::AUTO);
    static unsigned getSize(const Asset& asset);
    static std::shared_ptr<char[]> load(const Asset& asset);
};

} // namespace york::asset

#endif // YORK_ASSET_HPP
