//
// Created by Matthew McCall on 1/16/22.
//

#ifndef YORK_GRAPHICS_COMMANDPOOL_HPP
#define YORK_GRAPHICS_COMMANDPOOL_HPP

#include <vulkan/vulkan.hpp>

#include "Handle.hpp"
#include "Device.hpp"

namespace york::graphics {

class CommandPool : public Handle<vk::CommandPool> {
public:
    explicit CommandPool(Device& device);

private:
protected:
    bool createImpl() override;
    void destroyImpl() override;

private:
    Device& m_device;
};

}


#endif // YORK_GRAPHICS_COMMANDPOOL_HPP
