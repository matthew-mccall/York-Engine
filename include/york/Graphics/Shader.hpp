//
// Created by Matthew McCall on 12/18/21.
//

#ifndef YORK_GRAPHICS_SHADER_HPP
#define YORK_GRAPHICS_SHADER_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <vulkan/vulkan.hpp>

#include "Device.hpp"
#include "Handle.hpp"

namespace york::graphics {

/**
 * Represents a SPIR-V shaders for use with a Pipeline
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
     * @param m_device The device to create the shader with.
     * @param glsl The GLSL source to be compiled into SPIR-V.
     * @param type The type of shader to compile and bind to the pipeline.
     */
    explicit Shader(Device& m_device, std::string glsl, Type type);

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
    Device& m_device;
    Type m_type;
    std::string m_glsl;
};

}

#endif // YORK_GRAPHICS_SHADER_HPP
