//
// Created by Matthew McCall on 12/23/21.
//

#ifndef YORK_GRAPHICS_SWAPCHAIN_HPP
#define YORK_GRAPHICS_SWAPCHAIN_HPP

#include <vulkan/vulkan.hpp>

#include "Device.hpp"
#include "Handle.hpp"

namespace york::graphics {

class SwapChain : public Handle<vk::SwapchainKHR> {
public:
    SwapChain(Device& device, Window& window, Surface& surface);
    [[nodiscard]] const vk::Extent2D& getExtent() const;
    [[nodiscard]] Device& getDevice() const;
    [[nodiscard]] vk::SurfaceFormatKHR getFormat() const;

private:
    bool createImpl() override;
    void destroyImpl() override;

    Device& m_device;
    Window& m_window;
    Surface& m_surface;
    vk::SurfaceFormatKHR m_surfaceFormat;
    vk::PresentModeKHR m_presentMode;
    vk::Extent2D m_extent;
    vk::SurfaceCapabilitiesKHR m_capabilities;
    std::vector<vk::Image> m_swapChainImages;
};

}

#endif // YORK_GRAPHICS_SWAPCHAIN_HPP
