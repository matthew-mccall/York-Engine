//
// Created by Matthew McCall on 1/16/22.
//

#ifndef YORK_GRAPHICS_SEMAPHORE_HPP
#define YORK_GRAPHICS_SEMAPHORE_HPP

#include "Handle.hpp"
#include "Device.hpp"

#include <vulkan/vulkan.hpp>

namespace york::graphics {

class Semaphore : public Handle<vk::Semaphore> {
public:
    explicit Semaphore(Device& device);

protected:
    bool createImpl() override;
    void destroyImpl() override;

private:
    Device& m_device;
};

}

#endif // YORK_GRAPHICS_SEMAPHORE_HPP
