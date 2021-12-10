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
        m_instance.addDependent(*this);
    }

    bool Device::createImpl()
    {

        requestExtension({"VK_KHR_portability_subset", false});

        std::vector<vk::PhysicalDevice> physicalDevices = m_instance->enumeratePhysicalDevices();

        if (physicalDevices.empty()) {
            log::error("Could not find any supported GPUs!");
            return false;
        }

        std::vector<PhysicalDevice> sortedPhysicalDevices;
        sortedPhysicalDevices.reserve(physicalDevices.size());

        for (vk::PhysicalDevice& physicalDevice : physicalDevices) {
            sortedPhysicalDevices.emplace_back(physicalDevice, m_requestedExtensions);
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

        m_physicalDevice = sortedPhysicalDevices.front();

        float queuePriorities = 1.0;

        vk::DeviceQueueCreateInfo queueCreateInfo = { {}, sortedPhysicalDevices.front().getGraphicsFamilyQueueIndex(), 1, &queuePriorities };
        vk::PhysicalDeviceFeatures physicalDeviceFeatures;

        std::array<vk::DeviceQueueCreateInfo, 1> queueCreateInfos = { queueCreateInfo };

        std::vector<const char*> enabledExtensions(sortedPhysicalDevices.front().getEnabledExtensions().size());

        for (int i = 0; i < enabledExtensions.size(); i++) {
            enabledExtensions[i] = sortedPhysicalDevices.front().getEnabledExtensions()[i].c_str();
        }

        vk::DeviceCreateInfo createInfo = {{}, queueCreateInfos, {}, enabledExtensions, &physicalDeviceFeatures};

        m_handle = m_physicalDevice->createDevice(createInfo);

        return true;
    }

    void Device::requestExtension(const DeviceExtension& extension)
    {
        m_requestedExtensions.push_back((extension));
    }

    vk::PhysicalDevice Device::getPhysicalDevice() const
    {
        return m_physicalDevice.getPhysicalDevice();
    }

    void Device::destroyImpl()
    {
        m_handle.destroy();
    }

    Device::~Device()
    {
        m_instance.removeDependent(*this);
    }

    }