//
// Created by Matthew McCall on 1/7/22.
//

#include "york/Graphics/Framebuffer.hpp"

namespace york::graphics {

Framebuffer::Framebuffer(RenderPass& renderPass, ImageView& imageView)
    : m_renderPass(renderPass)
    , m_imageView(imageView)
    , m_device(m_renderPass.getDevice())
{
    addDependency(m_renderPass);
    addDependency(m_imageView);
}

bool Framebuffer::createImpl()
{
    std::array<vk::ImageView, 1> attachments { *m_imageView };

    vk::FramebufferCreateInfo framebufferCreateInfo {
        {},
        *m_renderPass,
        attachments,
        m_renderPass.getSwapChain().getExtent().width,
        m_renderPass.getSwapChain().getExtent().height,
        1
    };

    m_handle = m_device->createFramebuffer(framebufferCreateInfo);

    return true;
}
void Framebuffer::destroyImpl()
{
    m_device->destroy(m_handle);
}

}