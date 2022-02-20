//
// Created by Matthew McCall on 1/6/22.
//

#ifndef YORK_GRAPHICS_PIPELINELAYOUT_HPP
#define YORK_GRAPHICS_PIPELINELAYOUT_HPP

#include <vulkan/vulkan.hpp>

#include "Device.hpp"
#include "Handle.hpp"

namespace york::graphics {

/**
 * Handle wrapper for Vulkan Pipeline Layout
 */
class PipelineLayout : public Handle<vk::PipelineLayout> {
public:
    explicit PipelineLayout(Device& device);

protected:
    bool createImpl() override;
    void destroyImpl() override;

private:
    Device& m_device;
};

}


#endif // YORK_GRAPHICS_PIPELINELAYOUT_HPP
