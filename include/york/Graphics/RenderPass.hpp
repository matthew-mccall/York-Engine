//
// Created by Matthew McCall on 1/6/22.
//

#ifndef YORK_GRAPHICS_RENDERPASS_HPP
#define YORK_GRAPHICS_RENDERPASS_HPP

#include <vulkan/vulkan.hpp>

#include "SwapChain.hpp"
#include "Handle.hpp"

namespace york::graphics {

class RenderPass : public Handle<vk::RenderPass> {
public:
    explicit RenderPass(SwapChain& swapChain);

protected:
    bool createImpl() override;
    void destroyImpl() override;

private:
    SwapChain& m_swapChain;
    Device& m_device; // Device may change
};

}


#endif // YORK_GRAPHICS_RENDERPASS_HPP
