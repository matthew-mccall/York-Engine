//
// Created by Matthew McCall on 1/8/22.
//

#include "york/Renderer/Renderer.hpp"
#include "york/Graphics/ImageView.hpp"
#include "york/Graphics/ShaderDB.hpp"
#include "york/Asset.hpp"
#include "york/Log.hpp"

#include <array>
#include <limits>

namespace york {

Renderer::Renderer(graphics::Window& window)
    : m_window(window)
    , m_instance()
    , m_surface(m_instance, m_window)
    , m_device(m_instance, m_surface)
    , m_swapchain(m_device, m_window, m_surface)
    , m_renderPass(m_device)
    , m_pipeline(m_swapchain, m_renderPass)
    , m_commandPool(m_device)
{
    york::StringAsset vert { YORK_DEFAULT_VERTEX_SHADER.data(), york::Asset::Type::SHADER_VERT_GLSL };

    if (vert->empty()) {
        YORK_CORE_ERROR("Failed to load vertex shader!");
        return;
    }

    york::StringAsset frag { YORK_DEFAULT_FRAGMENT_SHADER.data(), york::Asset::Type::SHADER_FRAG_GLSL };

    if (frag->empty()) {
        YORK_CORE_ERROR("Failed to load fragment shader!");
        return;
    }

    m_defaultShaders.emplace_back(m_device, vert);
    m_defaultShaders.emplace_back(m_device, frag);

    m_pipeline.setShaders(m_defaultShaders);

    m_instance.create();

    std::vector<graphics::ImageView>& imageViews = m_swapchain.getImageViews();
    m_maxFrames = imageViews.size();

    vk::CommandBufferAllocateInfo commandBufferAllocateInfo { *m_commandPool, vk::CommandBufferLevel::ePrimary, static_cast<uint32_t>(m_maxFrames) };
    m_commandBuffers = m_device->allocateCommandBuffers(commandBufferAllocateInfo);

    m_frames.reserve(m_maxFrames);
    m_fences.reserve(m_maxFrames);
    m_imageAvailableSemaphores.reserve(m_maxFrames);
    m_renderFinishedSemaphores.reserve(m_maxFrames);

    for (unsigned i = 0; i < m_maxFrames; i++) {
        m_fences.emplace_back(m_device);
        m_fences.back().create();

        m_imageAvailableSemaphores.emplace_back(m_device);
        m_imageAvailableSemaphores.back().create();

        m_renderFinishedSemaphores.emplace_back(m_device);
        m_renderFinishedSemaphores.back().create();
    }

    for (unsigned i = 0; i < m_maxFrames; i++) {
        m_frames.emplace_back(m_renderPass, imageViews[i], m_swapchain, m_commandBuffers[i]);
        m_frames.back().create();
    }
}

bool Renderer::draw()
{
    graphics::FrameData& frame = m_frames[m_frameIndex];

    std::array<vk::Fence, 1> fences = { *m_fences[m_frameIndex] };
    auto waitResult = m_device->waitForFences(fences, VK_TRUE, std::numeric_limits<std::uint64_t>::max());

    auto [result, imageIndex] = m_device->acquireNextImageKHR(*m_swapchain, std::numeric_limits<std::uint64_t>::max(), *m_imageAvailableSemaphores[m_frameIndex], VK_NULL_HANDLE);

    if (result == vk::Result::eErrorOutOfDateKHR) {
        recreateSwapChain();
        return false;
    } else if ((result != vk::Result::eSuccess) && (result != vk::Result::eSuboptimalKHR)) {
        log::core::error("Failed to get next image!");
        return false;
    }

    m_device->resetFences(fences);

    vk::CommandBuffer commandBuffer = m_commandBuffers[m_frameIndex];

    vk::CommandBufferBeginInfo commandBufferBeginInfo { vk::CommandBufferUsageFlagBits::eOneTimeSubmit };
    commandBuffer.begin(commandBufferBeginInfo);

    vk::Viewport viewport {
        0,
        0,
        static_cast<float>(m_swapchain.getExtent().width),
        static_cast<float>(m_swapchain.getExtent().height),
        0,
        1
    };

    std::array<vk::Viewport, 1> viewports { viewport };

    commandBuffer.setViewport(0, viewports);

    vk::ClearValue clearValue { vk::ClearColorValue().setFloat32({ 0, 0, 0, 0 }) };
    vk::RenderPassBeginInfo renderPassBeginInfo { *m_renderPass, *(frame.getFramebuffer()), { { 0, 0 }, m_swapchain.getExtent() }, clearValue };
    commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *m_pipeline);
    commandBuffer.draw(3, 1, 0, 0);

    commandBuffer.endRenderPass();
    commandBuffer.end();

    std::array<vk::Semaphore, 1> waitSemaphores { *m_imageAvailableSemaphores[m_frameIndex] };
    std::array<vk::Semaphore, 1> signalSemaphores { *m_renderFinishedSemaphores[m_frameIndex] };
    std::array<vk::PipelineStageFlags, 1> waitStages { vk::PipelineStageFlagBits::eColorAttachmentOutput };

    std::array<vk::CommandBuffer, 1> commandBuffers = { commandBuffer };

    vk::SubmitInfo submitInfo { waitSemaphores, waitStages, commandBuffers, signalSemaphores };

    std::array<vk::SubmitInfo, 1> submitInfos { submitInfo };

    m_device.getGraphicsQueue().submit(submitInfos, *m_fences[m_frameIndex]);

    std::array<vk::SwapchainKHR, 1> swapchains { *m_swapchain };
    std::array<std::uint32_t, 1> imageIndices = { imageIndex };

    vk::PresentInfoKHR presentInfo { signalSemaphores, swapchains, imageIndices };

    auto presentResult = m_device.getPresentQueue().presentKHR(presentInfo);

    if ((presentResult == vk::Result::eErrorOutOfDateKHR) || (presentResult == vk::Result::eSuboptimalKHR) || resize) {
        resize = false;
        recreateSwapChain();
    } else if (presentResult != vk::Result::eSuccess) {
        log::core::error("Failed to get next image!");
        return false;
    }

    ++m_frameIndex %= m_maxFrames;

    return true;
}

Renderer::~Renderer()
{
    if (m_device.isCreated()) {
        m_device->waitIdle();

        for (unsigned i = 0; i < m_maxFrames; i++) {
            m_frames[i].destroy();
            m_fences[i].destroy();
            m_imageAvailableSemaphores[i].destroy();
            m_renderFinishedSemaphores[i].destroy();
        }

        m_frames.clear();
        m_fences.clear();
        m_imageAvailableSemaphores.clear();
        m_renderFinishedSemaphores.clear();

        m_instance.destroy();
    }
}

void Renderer::onEvent(Event& e)
{
    if (e.getType() == Event::Type::WindowResize) {
        if (m_window.getWindowID() == e.getWindowID()) {
            resize = true;
        }
    }
}

void Renderer::recreateSwapChain()
{

    m_device->waitIdle();
    m_frames.clear();

    m_swapchain.create();
    m_renderPass.create();

    std::vector<graphics::ImageView>& imageViews = m_swapchain.getImageViews();
    m_maxFrames = imageViews.size();

    for (unsigned i = 0; i < m_maxFrames; i++) {
        m_frames.emplace_back(m_renderPass, imageViews[i], m_swapchain, m_commandBuffers[i]);
        m_frames.back().create();
    }
}

}
