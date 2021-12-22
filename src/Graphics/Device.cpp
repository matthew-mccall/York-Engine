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

Device::Device(Instance& instance)
    : m_instance(instance)
{
    addDependency(m_instance);
    requestExtension({ "VK_KHR_portability_subset", false });
}

bool Device::createImpl()
{
    std::optional<PhysicalDevice> physicalDevice = PhysicalDevice::getBest(*m_instance, m_requestedExtensions);

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

    return true;
}

void Device::requestExtension(const DeviceExtension& extension)
{
    m_requestedExtensions.push_back((extension));
}

vk::PhysicalDevice Device::getPhysicalDevice() const
{
    return m_physicalDevice->getPhysicalDevice();
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

}