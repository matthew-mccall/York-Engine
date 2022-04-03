#include <SDL2/SDL.h>

#include <utility>

#include "york/Window.hpp"

namespace york::graphics {

Window::Window(std::string name, unsigned width, unsigned height)
    : m_width(width)
    , m_height(height)
    , m_name(std::move(name))
    , m_handle(SDL_CreateWindow(m_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, static_cast<int>(m_width), static_cast<int>(m_height), SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE))
{
}

std::string Window::getName() const
{
    return m_name;
}

unsigned Window::getWindowID()
{
    return SDL_GetWindowID(m_handle);
}

bool Window::isOpen()
{
    return m_handle != nullptr;
}

void Window::close()
{
    if (!isOpen()) return;

    SDL_DestroyWindow(m_handle);
    m_handle = nullptr;
}

SDL_Window* Window::getHandle()
{
    return m_handle;
}

Window::~Window()
{
    close();
}

} // namespace york::graphics
