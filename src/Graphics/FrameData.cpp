//
// Created by Matthew McCall on 2/19/22.
//

#include "york/Graphics/FrameData.hpp"

namespace york::graphics {

FrameData::FrameData(RenderPass& renderPass, ImageView& imageView, SwapChain& swapChain, vk::CommandBuffer commandBuffer)
    : m_commandBuffer(commandBuffer)
    , m_swapChain(swapChain)
    , m_framebuffer(renderPass, imageView, m_swapChain)
{
    addDependent(m_framebuffer);
    // addDependency(m_swapChain);
}

Framebuffer& FrameData::getFramebuffer()
{
    return m_framebuffer;
}

vk::CommandBuffer FrameData::getCommandBuffer()
{
    return m_commandBuffer;
}

bool FrameData::createImpl()
{
    return true;
}

void FrameData::destroyImpl()
{
}

}