//
// Created by Matthew McCall on 12/9/21.
//

#ifndef YORK_GRAPHICS_PHYSICALDEVICE_HPP
#define YORK_GRAPHICS_PHYSICALDEVICE_HPP

#include <vulkan/vulkan.hpp>
#include "york/Graphics/RequestableItem.h"

namespace york::graphics {

class PhysicalDevice {
public:
    PhysicalDevice(vk::PhysicalDevice device, std::vector<RequestableItem>& requestedExtensions);

    [[nodiscard]] unsigned int getRequiredExtensionsSupported() const;
    [[nodiscard]] unsigned int getOptionalExtensionsSupported() const;
    [[nodiscard]] const vk::PhysicalDevice& getPhysicalDevice() const;
    [[nodiscard]] uint32_t getGraphicsFamilyQueueIndex() const;
    [[nodiscard]] const std::vector<std::string>& getEnabledExtensions() const;

private:
    unsigned m_requiredExtensionsSupported = 0;
    unsigned m_optionalExtensionsSupported = 0;
    std::uint32_t m_graphicsFamilyQueueIndex = -1;
    std::uint32_t m_maximumImageResolution;

public:
    [[nodiscard]] uint32_t getMaximumImageResolution() const;

    vk::PhysicalDevice* operator->()
    {
        return &m_physicalDevice;
    }

private:
    vk::PhysicalDevice m_physicalDevice;
    std::vector<std::string> m_enabledExtensions;

};

}

#endif // YORK_GRAPHICS_PHYSICALDEVICE_HPP
