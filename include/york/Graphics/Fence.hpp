//
// Created by Matthew McCall on 2/19/22.
//

#ifndef YORK_GRAPHICS_FENCE_HPP
#define YORK_GRAPHICS_FENCE_HPP

#include <vulkan/vulkan.hpp>

#include "Handle.hpp"
#include "Device.hpp"

namespace york::graphics {

class Fence : public Handle<vk::Fence> {
public:
    Fence(Device& device);

protected:
    bool createImpl() override;
    void destroyImpl() override;

private:
    Device& m_device;
};

}


#endif // YORK_GRAPHICS_FENCE_HPP
