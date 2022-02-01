//
// Created by Matthew McCall on 12/18/21.
//

#include <glslang/Include/ResourceLimits.h>
#include <glslang/SPIRV/GlslangToSpv.h>

#include <shaderc/shaderc.hpp>

#include <utility>

#include "york/Graphics/Shader.hpp"
#include "york/Log.hpp"

namespace york::graphics {

Shader::Shader(Device& m_device, std::string glsl, Type type)
    : m_device(m_device)
    , m_glsl(std::move(glsl))
    , m_type(type)
{
    addDependency(m_device);

}

Shader::Type Shader::getType() const
{
    return m_type;
}

bool Shader::createImpl()
{
    shaderc::Compiler compiler;
    shaderc_shader_kind kind;

    switch (m_type) {
    case Type::Vertex:
        kind = shaderc_shader_kind::shaderc_glsl_vertex_shader;
        break;

    case Type::Fragment:
        kind = shaderc_shader_kind::shaderc_glsl_fragment_shader;
        break;

    default:
        kind = shaderc_shader_kind::shaderc_glsl_infer_from_source;
        break;
    }

    auto result = compiler.CompileGlslToSpv(m_glsl, kind, "");

    if (result.GetCompilationStatus() == shaderc_compilation_status_success) {
        std::vector<std::uint32_t> spirv {result.begin(), result.end() };
        vk::ShaderModuleCreateInfo createInfo = { {}, spirv };
        m_handle = m_device->createShaderModule(createInfo);
        log::core::debug("Compiled shaders!");
        return true;
    }

    YORK_CORE_ERROR(result.GetErrorMessage());

    return false;
}

void Shader::destroyImpl()
{
    m_device->destroy(m_handle);
}

Shader::~Shader()
{
    destroy();
}

}