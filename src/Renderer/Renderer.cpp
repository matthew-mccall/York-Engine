//
// Created by Matthew McCall on 1/8/22.
//

#include "york/Renderer/Renderer.hpp"
#include "york/Asset.hpp"
#include "york/Graphics/ImageView.hpp"
#include "york/Log.hpp"

#include <array>
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
{
    addDependency(m_window);
    addDependency(m_swapchain);
    addDependency(m_renderPass);
    addDependency(m_commandPool);
    addDependency(m_pipeline);
    // addDependency(m_instance);
    // addDependency(m_surface);
    // addDependency(m_device);

    york::Asset vert { "shaders/shader.vert", york::Asset::Type::SHADER_VERT_GLSL };

    if (vert->empty()) {
        YORK_CORE_ERROR("Failed to load vertex shader!");
        return;
    }

    york::graphics::Shader vertShader { m_device, vert };

    york::Asset frag { "shaders/shader.frag", york::Asset::Type::SHADER_FRAG_GLSL };

    if (frag->empty()) {
        YORK_CORE_ERROR("Failed to load fragment shader!");
        return;
    }

    york::graphics::Shader fragShader { m_device, frag };

    std::vector<york::graphics::Shader> shaders { vertShader, fragShader };

    m_pipeline.setShaders(shaders);
}

bool Renderer::createImpl()
{
    std::vector<graphics::ImageView>& imageViews = m_swapchain.getImageViews();
    m_maxFrames = imageViews.size();

    vk::CommandBufferAllocateInfo commandBufferAllocateInfo { *m_commandPool, vk::CommandBufferLevel::ePrimary, static_cast<uint32_t>(m_maxFrames) };
    m_commandBuffers = m_device->allocateCommandBuffers(commandBufferAllocateInfo);

    m_frames.reserve(m_maxFrames);
    m_swapFences.resize(m_maxFrames, VK_NULL_HANDLE);

    for (unsigned i = 0; i < m_maxFrames; i++) {
        m_frames.emplace_back(m_renderPass, imageViews[i], m_commandBuffers[i]);
        m_frames.back().create();
    }

    return true;
}

bool Renderer::draw()
{
    if (isCreated()) {
        graphics::FrameData& frame = m_frames[m_frameIndex];

        std::array<vk::Fence, 1> fences = { *frame.getFence() };
        auto waitResult =  m_device->waitForFences(fences, VK_TRUE, std::numeric_limits<std::uint64_t>::max());

        auto imageIndex = m_device->acquireNextImageKHR(*m_swapchain, std::numeric_limits<std::uint64_t>::max(), *frame.getImageAvailableSemaphore(), VK_NULL_HANDLE);
        if (imageIndex.result != vk::Result::eSuccess) {
            log::core::error("Failed to get next image!");
            return false;
        }

        if (static_cast<VkFence>(m_swapFences[imageIndex.value]) != VK_NULL_HANDLE) {
            std::array<vk::Fence, 1> swapFences { m_swapFences[imageIndex.value] };
            auto swapWaitResult = m_device->waitForFences(fences, VK_TRUE, std::numeric_limits<std::uint64_t>::max());
        }

        m_swapFences[imageIndex.value] = *frame.getFence();

        vk::CommandBuffer commandBuffer = m_commandBuffers[m_frameIndex];

        vk::CommandBufferBeginInfo commandBufferBeginInfo { vk::CommandBufferUsageFlagBits::eOneTimeSubmit };
        commandBuffer.begin(commandBufferBeginInfo);

        vk::ClearValue clearValue { vk::ClearColorValue().setFloat32({ 0, 0, 0, 0 }) };
        vk::RenderPassBeginInfo renderPassBeginInfo { *m_renderPass, *(m_frames[m_frameIndex].getFramebuffer()), { { 0, 0 }, m_swapchain.getExtent() }, clearValue };
        commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *m_pipeline);
        commandBuffer.draw(3, 1, 0, 0);

        commandBuffer.endRenderPass();
        commandBuffer.end();

        std::array<vk::Semaphore, 1> waitSemaphores { *frame.getImageAvailableSemaphore() };
        std::array<vk::Semaphore, 1> signalSemaphores { *frame.getRenderFinishedSemaphore() };
        std::array<vk::PipelineStageFlags, 1> waitStages { vk::PipelineStageFlagBits::eColorAttachmentOutput };

        std::array<vk::CommandBuffer, 1> commandBuffers = { commandBuffer };

        vk::SubmitInfo submitInfo { waitSemaphores, waitStages, commandBuffers, signalSemaphores };

        std::array<vk::SubmitInfo, 1> submitInfos { submitInfo };

        m_device->resetFences(fences);

        m_device.getGraphicsQueue().submit(submitInfos, *frame.getFence());

        std::array<vk::SwapchainKHR, 1> swapchains { *m_swapchain };
        std::array<std::uint32_t, 1> imageIndices = { imageIndex.value };

        vk::PresentInfoKHR presentInfo { signalSemaphores, swapchains, imageIndices };

        auto presentResult = m_device.getPresentQueue().presentKHR(presentInfo);
        ++m_frameIndex %= m_maxFrames;

        return true;
    }

    york::log::error("Renderer has not been created!");
    return false;
}

void Renderer::destroyImpl()
{
    m_device->waitIdle();

    for (graphics::FrameData& frame : m_frames) {
        frame.destroy();
    }

    m_frames.clear();
}

Renderer::~Renderer()
{
    if (m_device.isCreated()) {
        m_device->waitIdle();
    }
}

}
