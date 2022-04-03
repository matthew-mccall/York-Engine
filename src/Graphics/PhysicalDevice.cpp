//
// Created by Matthew McCall on 12/9/21.
//

#include <cstdint>

#include <SDL2/SDL_vulkan.h>

#include "york/Graphics/PhysicalDevice.hpp"
#include "york/Log.hpp"

namespace york::graphics {

PhysicalDevice::PhysicalDevice(vk::PhysicalDevice device, Surface& surface, std::vector<RequestableItem>& requestedExtensions)
    : m_physicalDevice(device)
    , m_surface(surface)
{
    std::vector<vk::ExtensionProperties> availableDeviceExtensions;
    availableDeviceExtensions = m_physicalDevice.enumerateDeviceExtensionProperties();

    for (const york::RequestableItem& requestedExtension : requestedExtensions) {
        for (vk::ExtensionProperties availableExtension : availableDeviceExtensions) {
            if (requestedExtension.name == availableExtension.extensionName) {
                if (requestedExtension.required) {
                    m_requiredExtensionsSupported++;
                    m_enabledExtensions.emplace_back(availableExtension.extensionName.begin(), availableExtension.extensionName.end());
                    break;
                }
                m_optionalExtensionsSupported++;
                m_enabledExtensions.emplace_back(availableExtension.extensionName.begin(), availableExtension.extensionName.end());
                break;
            }
        }
    }

    m_queueFamilyProperties = m_physicalDevice.getQueueFamilyProperties();

    unsigned int graphicsQueueFamilyIndex = 0;
    for (const vk::QueueFamilyProperties& queueFamily : m_queueFamilyProperties) {
        if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
            m_graphicsFamilyQueueIndex = graphicsQueueFamilyIndex;
        }
        graphicsQueueFamilyIndex++;
    }

    m_maximumImageResolution = device.getProperties().limits.maxImageDimension2D;

    m_formats = m_physicalDevice.getSurfaceFormatsKHR(*surface);
    std::vector<vk::PresentModeKHR> presentModes = m_physicalDevice.getSurfacePresentModesKHR(*surface);

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

uint32_t PhysicalDevice::getPresentFamilyQueueIndex()
{
    unsigned int presentQueueFamilyIndex = 0;
    for (const vk::QueueFamilyProperties& queueFamily : m_queueFamilyProperties) {
        if (m_physicalDevice.getSurfaceSupportKHR(presentQueueFamilyIndex, *m_surface) == VK_TRUE) {
            m_presentFamilyQueueIndex = presentQueueFamilyIndex;
            break;
        }
        presentQueueFamilyIndex++;
    }

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
std::optional<PhysicalDevice> PhysicalDevice::getBest(vk::Instance instance, Surface& surface, std::vector<RequestableItem>& requestedExtensions)
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

    std::sort(sortedPhysicalDevices.begin(), sortedPhysicalDevices.end(), [](PhysicalDevice& a, PhysicalDevice& b) -> bool {
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

vk::SurfaceCapabilitiesKHR PhysicalDevice::getSurfaceCapabilities()
{
    return m_physicalDevice.getSurfaceCapabilitiesKHR(*m_surface);
    ;
}

std::vector<vk::SurfaceFormatKHR>& PhysicalDevice::getFormats()
{
    m_formats = m_physicalDevice.getSurfaceFormatsKHR(*m_surface);
    return m_formats;
}

vk::SurfaceFormatKHR PhysicalDevice::getBestFormat()
{
    for (const auto& availableFormat : m_formats) {
        if (availableFormat.format == vk::Format::eB8G8R8Srgb && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
            return availableFormat;
        }
    }

    return getFormats()[0];
}

vk::PresentModeKHR PhysicalDevice::getBestPresentMode() const
{
    return m_presentBestMode;
}

}
