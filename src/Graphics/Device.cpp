//
// Created by Matthew McCall on 12/3/21.
//

#include <algorithm>
#include <array>
#include <vector>

#include "york/Graphics/Device.hpp"
#include "york/Log.hpp"

#include "york/Graphics/PhysicalDevice.hpp"

namespace york::graphics {

Device::Device(Instance& instance, Surface& surface)
    : m_instance(instance)
    , m_surface(surface)
{
    addDependency(m_instance);
    addDependency(m_surface);
    requestExtension({ "VK_KHR_portability_subset", false });
    requestExtension({ VK_KHR_SWAPCHAIN_EXTENSION_NAME });
}

bool Device::createImpl()
{
    std::optional<PhysicalDevice> physicalDevice = std::move(PhysicalDevice::getBest(*m_instance, *m_surface, m_requestedExtensions));

    if (!physicalDevice)
        return false;

    m_physicalDevice = physicalDevice;

    float queuePriorities = 1.0;

    vk::DeviceQueueCreateInfo queueCreateInfo { {}, m_physicalDevice->getGraphicsFamilyQueueIndex(), 1, &queuePriorities };
    vk::PhysicalDeviceFeatures physicalDeviceFeatures;

    std::array<vk::DeviceQueueCreateInfo, 1> queueCreateInfos { queueCreateInfo };

    std::vector<const char*> enabledExtensions(m_physicalDevice->getEnabledExtensions().size());

    for (int i = 0; i < enabledExtensions.size(); i++) {
        enabledExtensions[i] = m_physicalDevice->getEnabledExtensions()[i].c_str();
    }

    vk::DeviceCreateInfo createInfo { {}, queueCreateInfos, {}, enabledExtensions, &physicalDeviceFeatures };

    m_handle = (*m_physicalDevice)->createDevice(createInfo);
    m_graphicsQueue = std::make_pair(m_physicalDevice->getGraphicsFamilyQueueIndex(), m_handle.getQueue(m_physicalDevice->getGraphicsFamilyQueueIndex(), 0));
    m_presentQueue = std::make_pair(m_physicalDevice->getPresentFamilyQueueIndex(), m_handle.getQueue(m_physicalDevice->getGraphicsFamilyQueueIndex(), 0));

    return true;
}

void Device::requestExtension(const DeviceExtension& extension)
{
    m_requestedExtensions.push_back((extension));
}

PhysicalDevice& Device::getPhysicalDevice()
{
    return *m_physicalDevice;
}

void Device::destroyImpl()
{
    m_handle.destroy();
}

std::uint32_t Device::getGraphicsQueueIndex() const
{
    return m_graphicsQueue.first;
}

vk::Queue Device::getGraphicsQueue() const
{
    return m_graphicsQueue.second;
}

std::uint32_t Device::getPresentQueueIndex() const
{
    return m_presentQueue.first;
}

vk::Queue Device::getPresentQueue() const
{
    return m_presentQueue.second;
}

Surface& Device::getSurface()
{
    return m_surface;
}

}
