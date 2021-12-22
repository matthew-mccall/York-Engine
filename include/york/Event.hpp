#if !defined(YORK_EVENT_HPP)
#define YORK_EVENT_HPP

#include <variant>

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

    struct KeyData {
        KeyCode m_keyCode;
    };

    struct MouseButtonData {
        unsigned m_mouseButton;
    };

    struct MouseData {
        unsigned m_mouseX, m_mouseY;
    };

    struct TickData {
        float m_tickTime;
    };

    struct WindowData {
        unsigned m_windowID;
    };

    explicit Event(SDL_Event e);
    explicit Event(Type type);

    [[nodiscard]] Type getType() const;
    [[nodiscard]] KeyCode getKeyCode() const;
    [[nodiscard]] unsigned getMouseButton() const;
    [[nodiscard]] unsigned getMouseX() const;
    [[nodiscard]] unsigned getMouseY() const;
    [[nodiscard]] float getTickTime() const;
    [[nodiscard]] unsigned getWindowID() const;

    Type m_type;
    std::variant<KeyData, MouseButtonData, MouseData, TickData, WindowData> m_data;
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
