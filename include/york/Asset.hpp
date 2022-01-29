#if !defined(YORK_ASSET_HPP)
#define YORK_ASSET_HPP

#include <optional>
#include <memory>
#include <functional>
#include <string>
#include <variant>

#include "Async.hpp"

namespace york {

/**
 * An Asset is any file that can be loaded by the engine.
 */
class Asset {
public:

    enum class Type {
        IMG_PNG,
        SHADER_FRAG_GLSL,
        SHADER_FRAG_SPIRV,
        SHADER_VERT_GLSL,
        SHADER_VERT_SPIRV,
        SHADER_TESC_GLSL,
        SHADER_TESC_SPIRV,
        SHADER_TESE_GLSL,
        SHADER_TESE_SPIRV,
        SHADER_GEOM_GLSL,
        SHADER_GEOM_SPIRV,
        SHADER_COMP_GLSL,
        SHADER_COMP_SPIRV,
        TOML,
        UTF8,
        RAW,
        AUTO
    };

    enum class Source {
        FILE,
        NETWORK,
        MEMORY
    };

    /**
     * Creates an Asset handle, but does not load it.
     *
     * @param location A relative or absolute filepath to the asset, or a URL to a network location.
     * @param type The type of asset
     */
    explicit Asset(const std::string& location, Type type = Type::AUTO, Source source = Source::FILE);
    [[nodiscard]] Type getType() const;
    std::optional<std::reference_wrapper<std::vector<char>>> getData();

    std::optional<std::reference_wrapper<std::vector<char>>> operator*();
    std::vector<char>* operator->();

    char& operator[](std::size_t idx);

    ~Asset();

private:
    std::string m_location;
    std::vector<char>& m_data;
    Type m_type;
    Source m_source;
    int m_size;

};

} // namespace york::asset

#endif // YORK_ASSET_HPP
