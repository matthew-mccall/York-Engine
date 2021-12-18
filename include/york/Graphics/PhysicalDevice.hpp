//
// Created by Matthew McCall on 12/9/21.
//

#ifndef YORK_GRAPHICS_PHYSICALDEVICE_HPP
#define YORK_GRAPHICS_PHYSICALDEVICE_HPP

#include <optional>

#include <vulkan/vulkan.hpp>

#include "york/Graphics/RequestableItem.hpp"

namespace york::graphics {

/**
 * @brief A class representing properties about a physical GPU.
 *
 * Not to be confused with Device. Device actually initializes the GPU and interacts with it. PhysicalDevice gets properties about it.
 */
class PhysicalDevice {
public:
    /**
     * Creates a class representing information about a specific GPU.
     *
     * @param device The Vulkan handle for the physical device to get the information about.
     * @param requestedExtensions The extensions to check against this device's support for.
     */
    PhysicalDevice(vk::PhysicalDevice device, std::vector<RequestableItem>& requestedExtensions);

    /**
     * Gets the number of requested required extensions supported.
     *
     * @return The number of requested required extensions supported.
     */
    [[nodiscard]] unsigned int getRequiredExtensionsSupported() const;

    /**
     * Gets the number of requested optional extensions supported.
     *
     * @return The number of requested optional extensions supported.
     */
    [[nodiscard]] unsigned int getOptionalExtensionsSupported() const;

    /**
     * Gets the Vulkan handle for the device.
     *
     * @return The Vulkan handle for the device.
     */
    [[nodiscard]] const vk::PhysicalDevice& getPhysicalDevice() const;

    /**
     * Gets the index of the graphics queue of the device.
     *
     * @return The index of the graphics queue of the device.
     */
    [[nodiscard]] uint32_t getGraphicsFamilyQueueIndex() const;

    /**
     * Gets a list of the requested extensions that are supported by the device
     *
     * @return A list of the requested extensions that are supported by the device
     */
    [[nodiscard]] const std::vector<std::string>& getEnabledExtensions() const;

    /**
     * Gets the highest resolution supported by the device.
     *
     * @return The highest resolution supported by the device.
     */
    [[nodiscard]] uint32_t getMaximumImageResolution() const;

    /**
     * @brief Returns the best physical device.
     *
     * It gets a list of all physical devices supported by the instance. Then it checks them against
     * requestedExtensions and sorts them by whether it is a discrete GPU, extension compatibility, and supported
     * resolutions.
     *
     * @param instance The instance to get the physical devices from.
     * @param requestedExtensions The extensions to check the devices against.
     * @return
     */
    static std::optional<PhysicalDevice> getBest(vk::Instance instance, std::vector<RequestableItem>& requestedExtensions);

    vk::PhysicalDevice* operator->()
    {
        return &m_physicalDevice;
    }

private:
    unsigned m_requiredExtensionsSupported = 0;
    unsigned m_optionalExtensionsSupported = 0;
    std::uint32_t m_graphicsFamilyQueueIndex = -1;
    std::uint32_t m_maximumImageResolution;

    vk::PhysicalDevice m_physicalDevice;
    std::vector<std::string> m_enabledExtensions;

};

}

#endif // YORK_GRAPHICS_PHYSICALDEVICE_HPP
