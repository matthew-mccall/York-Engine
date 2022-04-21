//
// Created by Matthew McCall on 12/18/21.
//

#ifndef YORK_GRAPHICS_SHADER_HPP
#define YORK_GRAPHICS_SHADER_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <vulkan/vulkan.hpp>

#include "york/Asset.hpp"

#include "Device.hpp"
#include "Handle.hpp"

namespace york::graphics {

/**
 * Represents a SPIR-V shader for use with a Pipeline
 */
class Shader : public Handle<vk::ShaderModule> {
public:

    /**
     * Represents different types of shaders
     */
    enum class Type {
        Vertex,
        TessellationControl,    // Future support planned.
        TessellationEvaluation, // Future support planned.
        Geometry,               // Future support planned.
        Fragment,
        Compute,        // Future support planned.
        RTRayGen,       // Future support planned.
        RTAnyHit,       // Future support planned.
        RTClosestHit,   // Future support planned.
        RTMiss,         // Future support planned.
        RTIntersection, // Future support planned.
        RTCallable      // Future support planned.
    };

    /**
     * Creates a new shader object
     *
     * @param device The device to create the shader with.
     * @param asset The GLSL source to be compiled into SPIR-V.
     * @param type The type of shader to compile and bind to the pipeline.
     */
    explicit Shader(Device& device, const Asset& asset);

    /**
     * Compiles the GLSL into SPIR-V and creates the Vulkan Shader Module.
     *
     * @return Whether the module was successfully created.
     */
    bool createImpl() override;
    void destroyImpl() override;

    /**
     * @return The type of the shader.
     */
    [[nodiscard]] Type getType() const;

    ~Shader() override;

private:
    Asset m_asset;
    Device& m_device;
    Type m_type;
};

}

#endif // YORK_GRAPHICS_SHADER_HPP
