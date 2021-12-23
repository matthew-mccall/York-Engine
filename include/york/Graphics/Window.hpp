#if !defined(YORK_GRAPHICS_HPP)
#define YORK_GRAPHICS_HPP

#include <string>

#include <SDL_video.h>

#include "york/Graphics/Handle.hpp"

namespace york::graphics {

/**
 * A graphical window to which we can render to.
 */
class Window : public Handle<SDL_Window*> {
public:
    /**
     * Creates a Window.
     *
     * @param name The name of the window to go in the titlebar.
     * @param width The initial width of the window.
     * @param height The initial height of the window.
     */
    explicit Window(std::string name, unsigned width = 800, unsigned height = 600);

    /**
     * Gets the name of the window.
     *
     * @return  The name of the window.
     */
    [[nodiscard]] std::string getName() const;

    /**
     * Gets the identification number of the window.
     *
     * @return The ID number of the window.
     */
    unsigned getWindowID();

private:
    std::string m_name;
    unsigned m_width, m_height;

    bool createImpl() override;
    void destroyImpl() override;
};

} // namespace york::graphics

#endif
