#if !defined(YORK_ASSET_HPP)
#define YORK_ASSET_HPP

#include <optional>
#include <memory>
#include <functional>
#include <string>
#include <variant>

namespace york {

/**
 * An Asset is any file that can be loaded by the engine.
 */
class Asset {
public:

    /**
     * The type of Asset.
     *
     * This can allow the engine to automatically decode the file upon loading.
     */
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
        XML,
        UTF8,
        RAW,
        AUTO /**< Infer asset type from file extension */
    };

    /**
     * Where to load the file from.
     */
    enum class Source {
        FILE, /**< Load file from local filesystem */
        NETWORK, /**< Download file from the internet, assumes working internet connection */
        MEMORY /**< Copy from existing memory */
    };

    /**
     * Creates an Asset handle, but does not load it.
     *
     * @param location A relative or absolute filepath to the asset, or a URL to a network location.
     * @param type The type of asset
     */
    explicit Asset(const std::string& location, Type type = Type::AUTO, Source source = Source::FILE);

    /**
     * Gets the type of the asset
     * @return The type of the asset
     */
    [[nodiscard]] Type getType() const;

    /**
     * Returns the loaded data, if available.
     *
     * This may attempt to load the data on the spot if it is the first time called, previously failed, or the data was unloaded.
     * @return
     */
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
