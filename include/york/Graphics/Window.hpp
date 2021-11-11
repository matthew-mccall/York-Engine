#if !defined(YORK_GRAPHICS_HPP)
#define YORK_GRAPHICS_HPP

#include <SDL_video.h>

#include <string>

namespace york::graphics {

class Window {
public:
    Window(const std::string& name, unsigned width = 800, unsigned height = 600);
    int getID();
    ~Window();

private:
    SDL_Window* m_window;
};

} // namespace york::graphics

#endif
