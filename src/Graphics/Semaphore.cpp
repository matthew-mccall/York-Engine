//
// Created by Matthew McCall on 1/16/22.
//

#include "york/Graphics/Semaphore.hpp"

namespace york::graphics {

Semaphore::Semaphore(Device& device) : m_device(device) {
    addDependency(m_device);
}
bool Semaphore::createImpl()
{
    vk::SemaphoreCreateInfo createInfo {};
    m_handle = m_device->createSemaphore(createInfo);

    return true;
}
void Semaphore::destroyImpl()
{
    m_device->destroy(m_handle);
}

}
