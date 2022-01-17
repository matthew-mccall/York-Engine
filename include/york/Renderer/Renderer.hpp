//
// Created by Matthew McCall on 1/8/22.
//

#ifndef YORK_RENDERER_RENDERER_HPP
#define YORK_RENDERER_RENDERER_HPP

#include "york/Graphics/Framebuffer.hpp"
#include "york/Window.hpp"
#include "york/Graphics/Pipeline.hpp"
#include "york/Graphics/CommandPool.hpp"
#include "york/Graphics/Semaphore.hpp"

namespace york {

class Renderer : public graphics::HandleBase {
public:
    explicit Renderer(graphics::Window& window);
    void draw();

protected:
    bool createImpl() override;
    void destroyImpl() override;

private:
    york::graphics::Window& m_window;
    york::graphics::Instance m_instance;
    york::graphics::Surface m_surface;
    york::graphics::Device m_device;
    york::graphics::SwapChain m_swapchain;
    york::graphics::RenderPass m_renderPass;
    york::graphics::Pipeline m_pipeline;
    york::graphics::CommandPool m_commandPool;
    york::graphics::Semaphore m_imageAvailableSemaphore, m_renderFinishedSemaphore;
    std::vector<york::graphics::Framebuffer> m_framebuffers;
    std::vector<vk::CommandBuffer> m_commandBuffers;
};

}


#endif // YORK_RENDERER_RENDERER_HPP
