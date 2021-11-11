#include "york/Graphics/Window.hpp"

#include "SDL.h"

namespace {
static unsigned s_windowCount = 0;
} // namespace

namespace york::graphics {

Window::Window(const std::string& name, unsigned width, unsigned height)
{
    if (!s_windowCount) {
        SDL_Init(SDL_INIT_VIDEO);
    }

    m_window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_VULKAN);

    s_windowCount++;
}

int Window::getID()
{
    return SDL_GetWindowID(m_window);
}

Window::~Window()
{
    SDL_DestroyWindow(m_window);
}

} // namespace york::graphics
