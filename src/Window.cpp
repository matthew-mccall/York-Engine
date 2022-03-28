#include <SDL2/SDL.h>

#include <utility>

#include "york/Window.hpp"

namespace york::graphics {

Window::Window(std::string name, unsigned width, unsigned height)
    : m_name(std::move(name))
    , m_width(width)
    , m_height(height)
{
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

unsigned Window::getWindowID()
{
    return SDL_GetWindowID(m_handle);
}

} // namespace york::graphics
