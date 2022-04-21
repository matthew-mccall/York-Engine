//
// Created by Matthew McCall on 1/8/22.
//

#ifndef YORK_RENDERER_RENDERER_HPP
#define YORK_RENDERER_RENDERER_HPP

#include "york/Event.hpp"
#include "york/Window.hpp"
#include "york/Graphics/Framebuffer.hpp"
#include "york/Graphics/Pipeline.hpp"
#include "york/Graphics/CommandPool.hpp"
#include "york/Graphics/Semaphore.hpp"
#include "york/Graphics/FrameData.hpp"

namespace york {

/**
 * The Renderer allows you to draw content to a Window.
 */
class Renderer : public EventHandler{
public:
    /**
     * Create a Renderer bound to a window.
     *
     * @param window The window to bind to.
     */
    explicit Renderer(graphics::Window& window);

    /**
     * Draws to the window bound to.
     *
     * By default the renderer uses tripled buffer v-sync.
     */
    bool draw();

    void onEvent(Event &e) override;

    virtual ~Renderer();

private:
    york::graphics::Window& m_window;
    york::graphics::Instance m_instance;
    york::graphics::Surface m_surface;
    york::graphics::Device m_device;
    york::graphics::SwapChain m_swapchain;
    york::graphics::RenderPass m_renderPass;
    york::graphics::Pipeline m_pipeline;
    york::graphics::CommandPool m_commandPool;

    std::vector<graphics::FrameData> m_frames;
    std::vector<graphics::Fence> m_fences;
    std::vector<graphics::Semaphore> m_imageAvailableSemaphores, m_renderFinishedSemaphores;
    std::vector<graphics::Shader> m_defaultShaders;

    std::vector<vk::CommandBuffer> m_commandBuffers;

    std::uint32_t m_frameIndex = 0;
    std::uint32_t m_maxFrames = 0;

    bool resize = false;
};

}


#endif // YORK_RENDERER_RENDERER_HPP
