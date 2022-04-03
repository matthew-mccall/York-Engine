//
// Created by Matthew McCall on 1/7/22.
//

#ifndef YORK_GRAPHICS_FRAMEBUFFER_HPP
#define YORK_GRAPHICS_FRAMEBUFFER_HPP

#include "Handle.hpp"
#include "RenderPass.hpp"
#include "ImageView.hpp"

#include <vulkan/vulkan.hpp>

namespace york::graphics {

/**
 * Handle wrapper for Vulkan Framebuffer
 */
class Framebuffer : public Handle<vk::Framebuffer> {
public:
    explicit Framebuffer(RenderPass& renderPass, ImageView& imageView, SwapChain& swapChain);

protected:
    bool createImpl() override;
    void destroyImpl() override;

private:
    RenderPass& m_renderPass;
    ImageView& m_imageView;
    Device& m_device;
    SwapChain& m_swapChain;
};

}


#endif // YORK_GRAPHICS_FRAMEBUFFER_HPP
