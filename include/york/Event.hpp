#if !defined(YORK_EVENT_HPP)
#define YORK_EVENT_HPP

#include <variant>

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

    struct KeyData {
        KeyCode m_keyCode;
    };

    struct MouseButtonData {
        unsigned m_mouseButton;
    };

    struct MouseData {
        unsigned m_mouseX, m_mouseY;
    };

    struct WindowData {
        unsigned m_windowID;
    };

    Event() = delete;
    explicit Event(SDL_Event e);
    explicit Event(KeyData data, Type type);
    explicit Event(MouseButtonData data, Type type);
    explicit Event(MouseData data, Type type);
    explicit Event(WindowData data, Type type);

    [[nodiscard]] Type getType() const;
    [[nodiscard]] KeyCode getKeyCode() const;
    [[nodiscard]] unsigned getMouseButton() const;
    [[nodiscard]] unsigned getMouseX() const;
    [[nodiscard]] unsigned getMouseY() const;
    [[nodiscard]] unsigned getWindowID() const;

    Type m_type;
    std::variant<KeyData, MouseButtonData, MouseData, WindowData> m_data;
};

void pushEvent(SDL_Event e);
void pushEvent(const Event& e);
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
