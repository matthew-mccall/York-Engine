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
// Created by Matthew McCall on 1/3/22.
//

#ifndef YORK_GRAPHICS_PIPELINE_HPP
#define YORK_GRAPHICS_PIPELINE_HPP

#include <vulkan/vulkan.hpp>

#include "Handle.hpp"
#include "PipelineLayout.hpp"
#include "RenderPass.hpp"
#include "Shader.hpp"
#include "SwapChain.hpp"
#include "RenderPass.hpp"

namespace york::graphics {

/**
 * Handle wrapper for Vulkan Pipeline
 */
class Pipeline : public Handle<vk::Pipeline> {
public:
    explicit Pipeline(SwapChain& swapChain, RenderPass& renderPass, std::vector<Shader> shaders = {});
    void setShaders(std::vector<Shader> shaders);

protected:
    bool createImpl() override;
    void destroyImpl() override;

private:
    SwapChain& m_swapChain;
    Device& m_device;
    RenderPass& m_renderPass;

    std::vector<Shader> m_shaders;

    PipelineLayout m_pipelineLayout;
};

}


#endif // YORK_GRAPHICS_PIPELINE_HPP
