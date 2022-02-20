//
// Created by Matthew McCall on 2/19/22.
//

#include "york/Graphics/Fence.hpp"

namespace york::graphics {

Fence::Fence(Device& device) : m_device(device)
{
    addDependency(m_device);
}

bool Fence::createImpl()
{
    vk::FenceCreateInfo fenceCreateInfo { vk::FenceCreateFlagBits::eSignaled };
    m_handle = m_device->createFence(fenceCreateInfo);

    return true;
}
void Fence::destroyImpl()
{
    m_device->waitIdle();
    m_device->destroy(m_handle);
}

}