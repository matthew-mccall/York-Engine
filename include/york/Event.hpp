#if !defined(YORK_EVENT_HPP)
#define YORK_EVENT_HPP

#include <vector>
#include <functional>

#include <SDL_events.h>

#include "KeyCodes.hpp"
#include "Identifiable.hpp"
#include "Registry.hpp"

namespace york {

/**
 * Represents Event data
 */
struct Event {

    /**
     * The type of event.
     */
    enum class Type {
        None = 0,
        AppClosing,
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        WindowMoved,
        KeyPressed,
        KeyReleased,
        KeyTyped,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled,
        ControllerButtonPressed,
        ControllerButtonReleased
    };

    Event() = delete;
    explicit Event(SDL_Event e);

    /**
     * Gets the type of the event.
     *
     * @return The type of the event
     */
    [[nodiscard]] Type getType() const;

    /**
     * Gets the key code of the event.
     *
     * Assumes the event has key data. May fail otherwise.
     *
     * @return The key code of the event.
     */
    [[nodiscard]] KeyCode getKeyCode() const;

    /**
     * Gets the mouse button number of the event.
     *
     * Assumes the event has mouse button data.  May fail otherwise.
     *
     * @return
     */
    [[nodiscard]] unsigned getMouseButton() const;

    /**
     * Gets the horizontal component of the mouse data.
     *
     * Assumes the event has horizontal and vertical mouse data.  May fail otherwise.
     *
     * @return The horizontal component of the mouse data.
     */
    [[nodiscard]] unsigned getMouseX() const;

    /**
     * Gets the vertical component of the mouse data.
     *
     * Assumes the event has horizontal and vertical mouse data.  May fail otherwise.
     *
     * @return The vertical component of the mouse data.
     */
    [[nodiscard]] unsigned getMouseY() const;

    /**
     * Gets the window ID of the event
     *
     * Assumes the event has window data.
     *
     * @return The window ID of the event.
     */
    [[nodiscard]] unsigned getWindowID() const;

    Type m_type;
    union {
        struct { KeyCode m_keyCode; };
        struct { unsigned m_mouseButton; };
        struct { int m_mouseX, m_mouseY; };
        struct { unsigned m_windowID; };
    };
};

void pushEvent(SDL_Event e);

/**
 * Override this to handle events.
 */
class EventHandler : public Identifiable {
public:
    explicit EventHandler(Registry& registry);
    virtual void onEvent(Event& e) = 0;
    virtual ~EventHandler();

private:
    Registry& m_registry;
};

void dispatchEvents();

} // namespace york

#endif // YORK_EVENT_HPP
