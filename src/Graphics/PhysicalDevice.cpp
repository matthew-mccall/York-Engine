//
// Created by Matthew McCall on 12/9/21.
//

#include "york/Graphics/PhysicalDevice.hpp"
#include "york/Log.hpp"

namespace york::graphics {

PhysicalDevice::PhysicalDevice(vk::PhysicalDevice device, std::vector<RequestableItem>& requestedExtensions) : m_physicalDevice(device)
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

    m_maximumImageResolution = device.getProperties().limits.maxImageDimension2D;

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

const std::vector<std::string>& PhysicalDevice::getEnabledExtensions() const
{
    return m_enabledExtensions;
}

uint32_t PhysicalDevice::getMaximumImageResolution() const
{
    return m_maximumImageResolution;
}
std::optional<PhysicalDevice> PhysicalDevice::getBest(vk::Instance instance, std::vector<RequestableItem>& requestedExtensions)
{
    std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

    if (physicalDevices.empty()) {
        log::core::error("Could not find any supported GPUs!");
        return std::nullopt;
    }

    std::vector<PhysicalDevice> sortedPhysicalDevices;
    sortedPhysicalDevices.reserve(physicalDevices.size());

    for (vk::PhysicalDevice& physicalDevice : physicalDevices) {
        sortedPhysicalDevices.emplace_back(physicalDevice, requestedExtensions);
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
                if (a.getOptionalExtensionsSupported() > b.getOptionalExtensionsSupported()) {
                    return true;
                } else if (a.getOptionalExtensionsSupported() < b.getOptionalExtensionsSupported()) {
                    return false;
                } else {
                    return  a.getMaximumImageResolution() >= b.getMaximumImageResolution();
                }
            }
        }
    });

    return sortedPhysicalDevices.front();
}

}
