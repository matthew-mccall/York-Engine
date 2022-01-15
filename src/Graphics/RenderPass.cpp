//
// Created by Matthew McCall on 1/6/22.
//

#include "RenderPass.hpp"

namespace york::graphics {

RenderPass::RenderPass(SwapChain& swapChain) : m_swapChain(swapChain), m_device(swapChain.getDevice())
{
    addDependency(m_swapChain);
}

bool RenderPass::createImpl()
{
    vk::AttachmentDescription attachmentDescription {
        {},
        m_swapChain.getFormat().format,
        vk::SampleCountFlagBits::e1,
        vk::AttachmentLoadOp::eClear,
        vk::AttachmentStoreOp::eStore,
        vk::AttachmentLoadOp::eDontCare,
        vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::ePresentSrcKHR
    };

    vk::AttachmentReference attachmentReference {
        0,
        vk::ImageLayout::eColorAttachmentOptimal
    };

    std::array<vk::AttachmentReference, 1> attachmentReferences { attachmentReference };

    vk::SubpassDescription subpassDescription {
        {},
        vk::PipelineBindPoint::eGraphics,
        {},
        attachmentReferences
    };

    std::array<vk::AttachmentDescription, 1> colorAttachments { attachmentDescription };
    std::array<vk::SubpassDescription, 1> subpasses { subpassDescription };

    vk::RenderPassCreateInfo renderPassCreateInfo {
        {},
        colorAttachments,
        subpassDescription,
    };


    m_handle = m_device->createRenderPass(renderPassCreateInfo);

    return true;
}

void RenderPass::destroyImpl()
{
    m_device->destroy(m_handle);
}
Device& RenderPass::getDevice()
{
    return m_device;
}
SwapChain& RenderPass::getSwapChain()
{
    return m_swapChain;
}
}