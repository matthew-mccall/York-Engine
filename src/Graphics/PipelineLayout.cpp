//
// Created by Matthew McCall on 1/6/22.
//

#include "PipelineLayout.hpp"

namespace york::graphics {

PipelineLayout::PipelineLayout(Device& device) : m_device(device)
{
    addDependency(m_device);
}

bool PipelineLayout::createImpl()
{
    vk::PipelineLayoutCreateInfo layoutCreateInfo {};
    m_handle = m_device->createPipelineLayout(layoutCreateInfo);
    return true;
}

void PipelineLayout::destroyImpl()
{
    m_device->destroy(m_handle);
}

}

