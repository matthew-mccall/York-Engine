//
// Created by Matthew McCall on 1/16/22.
//

#include "york/Graphics/CommandPool.hpp"

namespace york::graphics {

CommandPool::CommandPool(Device& device)
    : m_device(device)
{
    addDependency(m_device);
}

bool CommandPool::createImpl()
{
    vk::CommandPoolCreateInfo createInfo {vk::CommandPoolCreateFlagBits::eResetCommandBuffer | vk::CommandPoolCreateFlagBits::eResetCommandBuffer, m_device.getGraphicsQueueIndex()};
    m_handle = m_device->createCommandPool(createInfo);

    return true;
}
void CommandPool::destroyImpl()
{
    m_device->destroy(m_handle);
}

}