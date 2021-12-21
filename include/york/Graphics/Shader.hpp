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

class Shader : public Handle<vk::ShaderModule> {
public:
    enum class Type {
        Vertex,
        TessellationControl,    // Currently unsupported.
        TessellationEvaluation, // Currently unsupported.
        Geometry,               // Currently unsupported.
        Fragment,
        Compute,        // Currently unsupported.
        RTRayGen,       // Currently unsupported.
        RTAnyHit,       // Currently unsupported.
        RTClosestHit,   // Currently unsupported.
        RTMiss,         // Currently unsupported.
        RTIntersection, // Currently unsupported.
        RTCallable
    };

    explicit Shader(Device& m_device, std::string& glsl, Type type);

    bool createImpl() override;
    void destroyImpl() override;

    ~Shader() override;

private:
    Device& m_device;
    Type m_type;
    std::vector<std::uint32_t> m_spirv;
    std::string m_glsl;
};

}

#endif // YORK_GRAPHICS_SHADER_HPP
