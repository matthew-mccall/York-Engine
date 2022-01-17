//
// Created by Matthew McCall on 12/9/21.
//

#include <limits>
#include <cstdint>

#include <SDL_vulkan.h>

#include "york/Graphics/PhysicalDevice.hpp"
#include "york/Log.hpp"

namespace york::graphics {

PhysicalDevice::PhysicalDevice(vk::PhysicalDevice device, vk::SurfaceKHR surface, std::vector<RequestableItem>& requestedExtensions)
    : m_physicalDevice(device)
{
    std::vector<vk::ExtensionProperties> availableDeviceExtensions;
    availableDeviceExtensions = m_physicalDevice.enumerateDeviceExtensionProperties();

    for (const york::RequestableItem& requestedExtension : requestedExtensions) {
        for (vk::ExtensionProperties availableExtension : availableDeviceExtensions) {
            if (requestedExtension.name == availableExtension.extensionName) {
                if (requestedExtension.required) {
                    m_requiredExtensionsSupported++;
                    m_enabledExtensions.emplace_back(availableExtension.extensionName);
                    break;
                }
                m_optionalExtensionsSupported++;
                m_enabledExtensions.emplace_back(availableExtension.extensionName);
                break;
            }
        }
    }

    std::vector<vk::QueueFamilyProperties> queueFamilies = m_physicalDevice.getQueueFamilyProperties();

    unsigned int graphicsQueueFamilyIndex = 0;
    for (const vk::QueueFamilyProperties& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
            m_graphicsFamilyQueueIndex = graphicsQueueFamilyIndex;
        }
        graphicsQueueFamilyIndex++;
    }

    unsigned int presentQueueFamilyIndex = 0;
    for (const vk::QueueFamilyProperties& queueFamily : queueFamilies) {
        if (m_physicalDevice.getSurfaceSupportKHR(presentQueueFamilyIndex, surface) == VK_TRUE) {
            m_presentFamilyQueueIndex = presentQueueFamilyIndex;
            break;
        }
        graphicsQueueFamilyIndex++;
    }

    m_maximumImageResolution = device.getProperties().limits.maxImageDimension2D;
    
    m_capabilities = m_physicalDevice.getSurfaceCapabilitiesKHR(surface);
    m_formats = m_physicalDevice.getSurfaceFormatsKHR(surface);
    std::vector<vk::PresentModeKHR> presentModes = m_physicalDevice.getSurfacePresentModesKHR(surface);

    m_presentBestMode = vk::PresentModeKHR::eFifo;

    for (auto& availablePresentMode : presentModes) {
        if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
            m_presentBestMode = availablePresentMode;
        }
    }

}

unsigned int PhysicalDevice::getRequiredExtensionsSupported() const
{
    return m_requiredExtensionsSupported;
}

unsigned int PhysicalDevice::getOptionalExtensionsSupported() const
{
    return m_optionalExtensionsSupported;
}

const vk::PhysicalDevice& PhysicalDevice::getPhysicalDevice() const
{
    return m_physicalDevice;
}

uint32_t PhysicalDevice::getGraphicsFamilyQueueIndex() const
{
    return m_graphicsFamilyQueueIndex;
}

uint32_t PhysicalDevice::getPresentFamilyQueueIndex() const
{
    return m_presentFamilyQueueIndex;
}

const std::vector<std::string>& PhysicalDevice::getEnabledExtensions() const
{
    return m_enabledExtensions;
}

uint32_t PhysicalDevice::getMaximumImageResolution() const
{
    return m_maximumImageResolution;
}
std::optional<PhysicalDevice> PhysicalDevice::getBest(vk::Instance instance, vk::SurfaceKHR surface, std::vector<RequestableItem>& requestedExtensions)
{
    std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

    if (physicalDevices.empty()) {
        log::core::error("Could not find any supported GPUs!");
        return std::nullopt;
    }

    std::vector<PhysicalDevice> sortedPhysicalDevices;
    sortedPhysicalDevices.reserve(physicalDevices.size());

    for (vk::PhysicalDevice& physicalDevice : physicalDevices) {
        sortedPhysicalDevices.emplace_back(physicalDevice, surface, requestedExtensions);
    }

    std::sort(sortedPhysicalDevices.begin(), sortedPhysicalDevices.end(), [](const PhysicalDevice& a, const PhysicalDevice& b) -> bool {
        if ((a.getPhysicalDevice().getProperties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu) && (b.getPhysicalDevice().getProperties().deviceType != vk::PhysicalDeviceType::eDiscreteGpu)) {
            return true;
        } else if ((a.getPhysicalDevice().getProperties().deviceType != vk::PhysicalDeviceType::eDiscreteGpu) && (b.getPhysicalDevice().getProperties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu)) {
            return false;
        } else {
            if (a.getRequiredExtensionsSupported() > b.getRequiredExtensionsSupported()) {
                return true;
            } else if (a.getRequiredExtensionsSupported() < b.getRequiredExtensionsSupported()) {
                return false;
            } else {
                if (a.getGraphicsFamilyQueueIndex() != -1 && b.getGraphicsFamilyQueueIndex() == -1) {
                    return true;
                } else if (a.getGraphicsFamilyQueueIndex() == -1 && b.getGraphicsFamilyQueueIndex() != -1) {
                    return false;
                } else {
                    if (a.getPresentFamilyQueueIndex() != -1 && b.getPresentFamilyQueueIndex() == -1) {
                        return true;
                    } else if (a.getPresentFamilyQueueIndex() == -1 && b.getPresentFamilyQueueIndex() != -1) {
                        return false;
                    } else {
                        if (a.getOptionalExtensionsSupported() > b.getOptionalExtensionsSupported()) {
                            return true;
                        } else if (a.getOptionalExtensionsSupported() < b.getOptionalExtensionsSupported()) {
                            return false;
                        } else {
                            return a.getMaximumImageResolution() >= b.getMaximumImageResolution();
                        }
                    }
                }
            }
        }
    });

    return sortedPhysicalDevices.front();
}

vk::SurfaceCapabilitiesKHR PhysicalDevice::getSurfaceCapabilities() {
    return m_capabilities;
}

std::vector<vk::SurfaceFormatKHR>& PhysicalDevice::getFormats() {
    return m_formats;
}

vk::SurfaceFormatKHR PhysicalDevice::getBestFormat() const {
    for (const auto& availableFormat : m_formats) {
            if (availableFormat.format == vk::Format::eB8G8R8Srgb && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                return availableFormat;
            }
        }

    return m_formats[0];
}

vk::PresentModeKHR PhysicalDevice::getBestPresentMode() const {
    return m_presentBestMode;
}

vk::Extent2D PhysicalDevice::getSwapExtent(Window& window) const {
    if (m_capabilities.currentExtent.width != std::numeric_limits<std::uint32_t>::max()) {
        return m_capabilities.currentExtent;
    } else {
        int width, height;
        SDL_Vulkan_GetDrawableSize(*window, &width, &height);
        
        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };
        
        actualExtent.width = std::clamp(actualExtent.width, m_capabilities.minImageExtent.width, m_capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, m_capabilities.minImageExtent.height, m_capabilities.maxImageExtent.height);
        
        return actualExtent;
    }
}

}
