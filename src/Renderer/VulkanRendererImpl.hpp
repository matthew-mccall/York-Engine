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
// Created by Matthew McCall on 1/8/22.
//

#ifndef YORK_VULKANRENDERERIMPL_HPP
#define YORK_VULKANRENDERERIMPL_HPP

#include "york/Renderer/RendererImpl.hpp"
#include "york/Event.hpp"

#include "Vulkan/CommandPool.hpp"
#include "Vulkan/FrameData.hpp"
#include "Vulkan/Framebuffer.hpp"
#include "Vulkan/Pipeline.hpp"
#include "Vulkan/Semaphore.hpp"

namespace york {

/**
 * The Renderer allows you to draw content to a Window.
 */
class VulkanRendererImpl : public EventHandler, public RendererImpl {
public:
    /**
     * Create a Renderer bound to a window.
     *
     * @param window The window to bind to.
     */
    explicit VulkanRendererImpl(Window& window);

    /**
     * Draws to the window bound to.
     *
     * By default the renderer uses tripled buffer v-sync.
     */
    bool draw() override;
    void onEvent(Event &e) override;

    ~VulkanRendererImpl() override;

private:
    york::graphics::Surface m_surface;
    york::graphics::Device m_device;
    york::graphics::SwapChain m_swapChain;
    york::graphics::RenderPass m_renderPass;
    york::graphics::Pipeline m_pipeline;
    york::graphics::CommandPool m_commandPool;

    Vector<graphics::Framebuffer> m_framebuffers;
    Vector<graphics::Fence> m_fences;
    Vector<graphics::Semaphore> m_imageAvailableSemaphores, m_renderFinishedSemaphores;
    Vector<graphics::Shader> m_defaultShaders;

    Vector<vk::CommandBuffer> m_commandBuffers;

    std::uint32_t m_frameIndex = 0;
    std::uint32_t m_maxFrames = 0;

    bool resize = false;

    void recreateSwapChain();
};

}


#endif // YORK_VULKANRENDERERIMPL_HPP
