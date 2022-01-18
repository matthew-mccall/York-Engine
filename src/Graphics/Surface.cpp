//
// Created by Matthew McCall on 12/21/21.
//

#include <SDL_vulkan.h>

#include "york/Graphics/Surface.hpp"
#include "york/Log.hpp"

namespace york::graphics {

Surface::Surface(Instance& instance, Window& window)
    : m_instance(instance)
    , m_window(window)
{
    addDependency(m_window);
    addDependency(m_instance);
}

bool Surface::createImpl()
{
    VkSurfaceKHR surface;
    VkInstance instance = *m_instance;

    if (!SDL_Vulkan_CreateSurface(m_window.getHandle(), instance, &surface)) {
        YORK_CORE_ERROR(SDL_GetError());
        return false;
    }

    m_handle = surface;
    return true;
}

void Surface::destroyImpl()
{
    m_instance->destroy(m_handle);
}

}