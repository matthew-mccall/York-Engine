//
// Created by Matthew McCall on 1/6/22.
//

#ifndef YORK_GRAPHICS_RENDERPASS_HPP
#define YORK_GRAPHICS_RENDERPASS_HPP

#include <vulkan/vulkan.hpp>

#include "Handle.hpp"
#include "SwapChain.hpp"

namespace york::graphics {

/**
 * Handle wrapper for Vulkan Render Pass
 */
class RenderPass : public Handle<vk::RenderPass> {
public:
    explicit RenderPass(Device& device);
    Device& getDevice();

protected:
    bool createImpl() override;
    void destroyImpl() override;

private:
    Device& m_device;
};

}


#endif // YORK_GRAPHICS_RENDERPASS_HPP
