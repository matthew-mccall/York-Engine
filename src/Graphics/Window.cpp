#include <SDL.h>

#include "york/Graphics/Window.hpp"
#include "york/Log.hpp"

namespace {
bool s_windowInit = false;
} // namespace

namespace york::graphics {

Window::Window(const std::string& name, unsigned width, unsigned height)
{
    if (!s_windowInit) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            log::core::error(SDL_GetError());
            return;
        }
    }

    m_handle = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_VULKAN);
}

bool Window::createImpl()
{
    return true;
}

void Window::destroyImpl()
{
}

std::string Window::getName() const
{
    return SDL_GetWindowTitle(m_handle);
}

unsigned Window::getID()
{
    return SDL_GetWindowID(m_handle);
}

Window::~Window()
{
    SDL_DestroyWindow(m_handle);
}

} // namespace york::graphics
