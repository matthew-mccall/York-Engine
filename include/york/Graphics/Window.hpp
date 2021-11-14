#if !defined(YORK_GRAPHICS_HPP)
#define YORK_GRAPHICS_HPP

#include <string>

#include <SDL_video.h>

#include "york/Graphics/Handle.hpp"

namespace york::graphics {

class Window : public Handle<SDL_Window*> {
public:
    Window(const std::string& name, unsigned width = 800, unsigned height = 600);
    std::string getName() const;
    int getID();
    ~Window();

private:
    bool createImpl() override;
    void destroyImpl() override;
};

} // namespace york::graphics

#endif
