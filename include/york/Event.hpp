#if !defined(YORK_EVENT_HPP)
#define YORK_EVENT_HPP

#include <SDL_events.h>

#include "KeyCodes.hpp"

namespace york {

struct Event {
    enum class Type {
        None = 0,
        AppTick,
        AppRender,
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

    Event(SDL_Event e);
    Event(Type type);

    Type getType() const;
    KeyCode getKeyCode() const;
    unsigned getMouseButton() const;
    unsigned getMouseX() const;
    unsigned getMouseY() const;
    float getTickTime() const;
    unsigned getWindowID() const;

    Type m_type;

    union {
        struct {
            KeyCode m_keyCode;
        };
        struct {
            unsigned m_mouseButton;
        };
        struct {
            unsigned m_mouseX, m_mouseY;
        };
        struct {
            float m_tickTime;
        };
        struct {
            unsigned m_windowID;
        };
    };
};

void pushEvent(SDL_Event e);
void pushEvent(Event e);
void dispatchEvents();

class EventHandler {
    friend void dispatchEvents();

public:
    EventHandler();
    ~EventHandler();

protected:
    virtual void onEvent(Event e) = 0;
};

} // namespace york

#endif // YORK_EVENT_HPP
