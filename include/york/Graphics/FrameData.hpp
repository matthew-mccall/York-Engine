//
// Created by Matthew McCall on 2/19/22.
//

#ifndef YORK_GRAPHICS_FRAMEDATA_HPP
#define YORK_GRAPHICS_FRAMEDATA_HPP

#include "Semaphore.hpp"
#include "CommandPool.hpp"
#include "Fence.hpp"
#include "Framebuffer.hpp"
#include "SwapChain.hpp"

namespace york::graphics {

/**
 * Contains the necessary synchronization data structures, framebuffer and command buffer for a frame.
 */
class FrameData : public HandleBase {
public:
    FrameData(RenderPass& renderPass, ImageView& imageView, SwapChain& swapChain, vk::CommandBuffer commandBuffer);

    [[nodiscard]] Framebuffer& getFramebuffer();
    [[nodiscard]] vk::CommandBuffer getCommandBuffer();

protected:
    bool createImpl() override;
    void destroyImpl() override;

private:
    vk::CommandBuffer m_commandBuffer;

    SwapChain& m_swapChain;
    Framebuffer m_framebuffer;
};

}

#endif // YORK_GRAPHICS_FRAMEDATA_HPP
