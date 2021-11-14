#include <vector>

#include <SDL_vulkan.h>

#include "york/Graphics/Instance.hpp"
#include "york/Log.hpp"

namespace york::graphics {

Instance::Instance(Window& window)
    : m_window(window)
{
}

bool Instance::createImpl()
{
    vk::ApplicationInfo appInfo = { m_window.getName().c_str(), VK_MAKE_VERSION(1, 0, 0), "York Engine", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_1 };

    unsigned extensionCount;

    if (!SDL_Vulkan_GetInstanceExtensions(m_window.getHandle(), &extensionCount, nullptr)) {
        log::core::error(SDL_GetError());
        return false;
    }

    std::vector<const char*> extensions(extensionCount);

    if (!SDL_Vulkan_GetInstanceExtensions(m_window.getHandle(), &extensionCount, extensions.data())) {
        log::core::error(SDL_GetError());
        return false;
    }

    vk::InstanceCreateInfo createInfo = { {}, &appInfo, {}, extensions };

    m_handle = vk::createInstance(createInfo);

    return true;
}

void Instance::destroyImpl()
{
    m_handle.destroy();
}

} // namespace york::graphics
