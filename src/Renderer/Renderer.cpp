//
// Created by Matthew McCall on 1/8/22.
//

#include "york/Asset.hpp"
#include "york/Log.hpp"
#include "york/Graphics/ImageView.hpp"
#include "york/Renderer/Renderer.hpp"

#include <limits>

namespace york {

Renderer::Renderer(graphics::Window& window)
    : m_window(window)
    , m_instance(m_window)
    , m_surface(m_instance, m_window)
    , m_device(m_instance, m_surface)
    , m_swapchain(m_device, m_window, m_surface)
    , m_renderPass(m_swapchain)
    , m_pipeline(m_swapchain, m_renderPass)
    , m_commandPool(m_device)
    , m_imageAvailableSemaphore(m_device)
    , m_renderFinishedSemaphore(m_device)
{
    addDependency(m_window);
    addDependency(m_swapchain);
    addDependency(m_renderPass);
    addDependency(m_commandPool);
    // addDependency(m_instance);
    // addDependency(m_surface);
    // addDependency(m_device);

    york::Asset vert { "test/assets/shader.vert", york::Asset::Type::SHADER_VERT_GLSL };
    std::string vertSrc = std::string { vert->data() };
    york::graphics::Shader vertShader { m_device, vertSrc, york::graphics::Shader::Type::Vertex };

    york::Asset frag { "test/assets/shader.frag", york::Asset::Type::SHADER_VERT_GLSL };
    std::string fragSrc = std::string { frag->data() };
    york::graphics::Shader fragShader { m_device, fragSrc, york::graphics::Shader::Type::Fragment };

    std::vector<york::graphics::Shader> shaders { vertShader, fragShader };

    m_pipeline.setShaders(shaders);
}

bool Renderer::createImpl()
{
    for (york::graphics::ImageView& imageView : m_swapchain.getImageViews()) {
        m_framebuffers.emplace_back(m_renderPass, imageView);
        m_framebuffers.back().create(); // TODO: Remove when command buffers are being recorded every frame
    }

    vk::CommandBufferAllocateInfo commandBufferAllocateInfo {*m_commandPool, vk::CommandBufferLevel::ePrimary, static_cast<uint32_t>(m_framebuffers.size()) };
    m_commandBuffers = m_device->allocateCommandBuffers(commandBufferAllocateInfo);

    for (unsigned i = 0; i < m_commandBuffers.size() && i < m_framebuffers.size(); i++) {
        vk::CommandBufferBeginInfo commandBufferBeginInfo {};
        m_commandBuffers[i].begin(commandBufferBeginInfo);

        vk::ClearValue clearValue { vk::ClearColorValue().setFloat32({0, 0, 0, 0}) };
        vk::RenderPassBeginInfo renderPassBeginInfo { *m_renderPass, *m_framebuffers[i], {{0, 0}, m_swapchain.getExtent()}, clearValue};
        m_commandBuffers[i].beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

        m_commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, *m_pipeline);
        m_commandBuffers[i].draw(3, 1, 0, 0);

        m_commandBuffers[i].endRenderPass();
        m_commandBuffers[i].end();

    }

    return true;
}

void Renderer::draw()
{
    if (isCreated()) {
        auto imageIndex = m_device->acquireNextImageKHR(*m_swapchain, std::numeric_limits<std::uint64_t>::max(), *m_imageAvailableSemaphore, VK_NULL_HANDLE);
        if (imageIndex.result != vk::Result::eSuccess) {
            log::core::error("Failed to get next image!");
            return;
        }
    }
}

void Renderer::destroyImpl()
{
    for (york::graphics::Framebuffer& framebuffer: m_framebuffers) {
        framebuffer.destroy();
    }
    m_framebuffers.clear();
}
}