/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2022 Matthew McCall
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

//
// Created by Matthew McCall on 12/18/21.
//

#include <shaderc/shaderc.hpp>

#include <utility>
#include <string>

#include "york/Asset.hpp"
#include "york/Graphics/Shader.hpp"
#include "york/Log.hpp"

namespace york::graphics {

Shader::Shader(Device& device, Asset& asset)
    : m_device(device)
    , m_asset(asset)
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
    std::string glsl;
    shaderc::CompilationResult<std::uint32_t> result;

    switch (m_asset.getType()) {
    case Asset::Type::SHADER_VERT_GLSL:
        m_type = Type::Vertex;
        goto compileShader;
    case Asset::Type::SHADER_FRAG_GLSL:
        m_type = Type::Fragment;
        goto compileShader;
    case Asset::Type::SHADER_COMP_GLSL:
        m_type = Type::Compute;
        goto compileShader;
    case Asset::Type::SHADER_GEOM_GLSL:
        m_type = Type::Geometry;
        goto compileShader;
    case Asset::Type::SHADER_TESE_GLSL:
        m_type = Type::TessellationEvaluation;
        goto compileShader;
    case Asset::Type::SHADER_TESC_GLSL:
        m_type = Type::TessellationControl;
        goto compileShader;
    case Asset::Type::SHADER_VERT_SPIRV:
        m_type = Type::Vertex;
        goto copyShader;
    case Asset::Type::SHADER_FRAG_SPIRV:
        m_type = Type::Fragment;
        goto copyShader;
    default:
        goto compileShader;
    }

compileShader:
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

    glsl = { m_asset->begin(), m_asset->end() };
    result = compiler.CompileGlslToSpv(glsl, kind, "");

    if (result.GetCompilationStatus() == shaderc_compilation_status_success) {
        std::vector<std::uint32_t> spirv { result.begin(), result.end() };
        vk::ShaderModuleCreateInfo createInfo = { {}, spirv };
        m_handle = m_device->createShaderModule(createInfo);
        YORK_CORE_DEBUG("Compiled shaders!");
        return true;
    }

    YORK_CORE_ERROR(result.GetErrorMessage());

    return false;

copyShader:
    vk::ShaderModuleCreateInfo createInfo = { {}, m_asset->size(), reinterpret_cast<std::uint32_t*>(m_asset->data()) };
    m_handle = m_device->createShaderModule(createInfo);
    YORK_CORE_DEBUG("Compiled shaders!");
    return true;
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