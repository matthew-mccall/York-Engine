#include <SDL.h>

#include <utility>

#include "york/Graphics/Window.hpp"
#include "york/Log.hpp"

namespace {
bool s_windowInit = false;
} // namespace

namespace york::graphics {

Window::Window(std::string name, unsigned width, unsigned height)
    : m_name(std::move(name))
    , m_width(width)
    , m_height(height)
{
    if (!s_windowInit) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            log::core::error(SDL_GetError());
            return;
        }

        s_windowInit = true;
    }
}

bool Window::createImpl()
{
    m_handle = SDL_CreateWindow(m_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, static_cast<int>(m_width), static_cast<int>(m_height), SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_VULKAN);
    return (m_handle != nullptr);
}

void Window::destroyImpl()
{
    SDL_DestroyWindow(m_handle);
}

std::string Window::getName() const
{
    return m_name;
}

unsigned Window::getID()
{
    return SDL_GetWindowID(m_handle);
}

} // namespace york::graphics
