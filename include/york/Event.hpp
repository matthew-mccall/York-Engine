#if !defined(YORK_EVENT_HPP)
#define YORK_EVENT_HPP

#include <SDL_events.h>

#include "KeyCodes.hpp"

namespace york {

struct Event {
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

    [[nodiscard]] Type getType() const;
    [[nodiscard]] KeyCode getKeyCode() const;
    [[nodiscard]] unsigned getMouseButton() const;
    [[nodiscard]] unsigned getMouseX() const;
    [[nodiscard]] unsigned getMouseY() const;
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
void dispatchEvents();

class EventHandler {
    friend void dispatchEvents();

public:
    EventHandler();
    ~EventHandler();

protected:
    virtual void onEvent(Event& e) = 0;
};

} // namespace york

#endif // YORK_EVENT_HPP
