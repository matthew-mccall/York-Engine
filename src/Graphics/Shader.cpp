//
// Created by Matthew McCall on 12/18/21.
//

#include <shaderc/shaderc.hpp>

#include <utility>

#include "york/Graphics/Shader.hpp"
#include "york/Log.hpp"
#include "york/Asset.hpp"

namespace york::graphics {

Shader::Shader(Device& m_device, const Asset& asset)
    : m_device(m_device)
    , m_asset(asset)
{
    addDependency(m_device);

    switch (asset.getType()) {
    case Asset::Type::SHADER_VERT_GLSL:
        m_type = Type::Vertex;
        break;
    case Asset::Type::SHADER_FRAG_GLSL:
        m_type = Type::Fragment;
        break;
    case Asset::Type::SHADER_COMP_GLSL:
        m_type = Type::Compute;
        break;
    case Asset::Type::SHADER_GEOM_GLSL:
        m_type = Type::Geometry;
        break;
    case Asset::Type::SHADER_TESE_GLSL:
        m_type = Type::TessellationEvaluation;
        break;
    case Asset::Type::SHADER_TESC_GLSL:
        m_type = Type::TessellationControl;
        break;
    default:
        break;
    }

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

    std::string glsl = { m_asset->data() };

    auto result = compiler.CompileGlslToSpv(glsl, kind, "");

    if (result.GetCompilationStatus() == shaderc_compilation_status_success) {
        std::vector<std::uint32_t> spirv {result.begin(), result.end() };
        vk::ShaderModuleCreateInfo createInfo = { {}, spirv };
        m_handle = m_device->createShaderModule(createInfo);
        YORK_CORE_DEBUG("Compiled shaders!");
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