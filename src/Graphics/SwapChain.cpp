//
// Created by Matthew McCall on 12/23/21.
//

#include <array>
#include <cstdint>

#include "york/Graphics/SwapChain.hpp"

namespace york::graphics {

SwapChain::SwapChain(Device& device, Window& window, Surface& surface)
    : m_device(device)
    , m_window(window)
    , m_surface(surface)
    , m_presentMode(m_device.getPhysicalDevice().getBestPresentMode())
{
    addDependency(m_device);
    addDependency(m_window);
    addDependency(m_surface);
}

bool SwapChain::createImpl()
{
    PhysicalDevice& physicalDevice = m_device.getPhysicalDevice();

    m_surfaceFormat = physicalDevice.getBestFormat();
    m_presentMode = physicalDevice.getBestPresentMode();
    m_extent = physicalDevice.getSwapExtent(m_window);
    m_capabilities = physicalDevice.getSurfaceCapabilities();

    std::uint32_t imageCount = m_capabilities.minImageCount + 1;
    std::uint32_t maxImageCount = m_capabilities.maxImageCount;

    if ((maxImageCount > 0 )&& (imageCount > maxImageCount)) {
        imageCount = maxImageCount;
    }

    vk::SwapchainCreateInfoKHR createInfo {
        {},
        *m_surface,
        imageCount,
        m_surfaceFormat.format,
        m_surfaceFormat.colorSpace,
        m_extent,
        1,
        vk::ImageUsageFlagBits::eColorAttachment,
        vk::SharingMode::eExclusive,
        0,
        {},
        m_capabilities.currentTransform,
        vk::CompositeAlphaFlagBitsKHR::eOpaque,
        m_presentMode,
        VK_TRUE,
        VK_NULL_HANDLE
    };

    std::array<std::uint32_t, 2> queueFamilyIndices = { physicalDevice.getGraphicsFamilyQueueIndex(), physicalDevice.getPresentFamilyQueueIndex() };

    if (queueFamilyIndices[0] != queueFamilyIndices[1]) {
        createInfo.setQueueFamilyIndices(queueFamilyIndices);
    }

    m_handle = m_device->createSwapchainKHR(createInfo);
    m_swapChainImages = m_device->getSwapchainImagesKHR(m_handle);

    for (vk::Image image : m_swapChainImages) {
        m_swapChainImageViews.emplace_back(m_device, m_surfaceFormat.format, image);
    }

    return true;
}

void SwapChain::destroyImpl()
{
    for (ImageView& imageView : m_swapChainImageViews) {
        imageView.destroy();
    }
    m_swapChainImageViews.clear();

    m_device->destroy(m_handle);
}
const vk::Extent2D& SwapChain::getExtent() const
{
    return m_extent;
}

Device& SwapChain::getDevice() const
{
    return m_device;
}

vk::SurfaceFormatKHR SwapChain::getFormat() const
{
    return m_surfaceFormat;
}

std::vector<ImageView>& SwapChain::getImageViews()
{
    return m_swapChainImageViews;
}

}
