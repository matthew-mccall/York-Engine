//
// Created by Matthew McCall on 2/19/22.
//

#include "york/Graphics/FrameData.hpp"

namespace york::graphics {

FrameData::FrameData(RenderPass& renderPass, ImageView& imageView, vk::CommandBuffer commandBuffer)
    : m_framebuffer(renderPass, imageView)
    , m_imageAvailableSemaphore(renderPass.getDevice())
    , m_renderFinishedSemaphore(renderPass.getDevice())
    , m_fence(renderPass.getDevice())
    , m_commandBuffer(commandBuffer)
{
    addDependent(m_framebuffer);
    addDependent(m_renderFinishedSemaphore);
    addDependent(m_imageAvailableSemaphore);
    addDependent(m_fence);
}

Semaphore& FrameData::getImageAvailableSemaphore()
{
    return m_imageAvailableSemaphore;
}

Semaphore& FrameData::getRenderFinishedSemaphore()
{
    return m_renderFinishedSemaphore;
}

Framebuffer& FrameData::getFramebuffer()
{
    return m_framebuffer;
}

vk::CommandBuffer FrameData::getCommandBuffer()
{
    return m_commandBuffer;
}

Fence& FrameData::getFence()
{
    return m_fence;
}

bool FrameData::createImpl()
{
    return true;
}

void FrameData::destroyImpl()
{
}


}