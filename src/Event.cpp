#include <cassert>
#include <queue>
#include <utility>
#include <vector>

#include "SDL_events.h"

#include "york/Event.hpp"

namespace {

std::queue<york::Event> s_eventQueue;

} // namespace

namespace york {

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"

Event::Event(SDL_Event e)
{
    switch (e.type) {

    case SDL_KEYDOWN:
        m_type = Type::KeyPressed;
        m_keyCode = static_cast<KeyCode>(e.key.keysym.scancode);
        break;

    case SDL_KEYUP:
        m_type = Type::KeyReleased;
        m_keyCode = static_cast<KeyCode>(e.key.keysym.scancode);
        break;

    case SDL_MOUSEBUTTONDOWN:
        m_type = Type::MouseButtonPressed;
        m_mouseButton = e.button.button;
        break;

    case SDL_MOUSEBUTTONUP:
        m_type = Type::MouseButtonReleased;
        m_mouseButton = e.button.button;
        break;

    case SDL_MOUSEMOTION:
        m_type = Type::MouseMoved;
        m_mouseX = e.motion.x;
        m_mouseY = e.motion.y;
        break;

    case SDL_MOUSEWHEEL:
        m_type = Type::MouseMoved;
        m_mouseX = e.wheel.x;
        m_mouseY = e.wheel.y;
        break;

    case SDL_WINDOWEVENT:

        switch (e.window.event) {

        case SDL_WINDOWEVENT_CLOSE:
            m_type = Type::WindowClose;
            m_windowID = e.window.windowID;
            break;

        default:
            break;
        }

        break;

    default:
        break;
    }
}

#pragma clang diagnostic pop

Event::Type Event::getType() const
{
    return m_type;
}

KeyCode Event::getKeyCode() const
{
    assert((m_type == Type::KeyPressed) | (m_type == Type::KeyReleased) | (m_type == Type::KeyTyped));
    return m_keyCode;
}

unsigned Event::getMouseButton() const
{
    assert((m_type == Type::MouseButtonPressed) | (m_type == Type::MouseButtonReleased));
    return m_mouseButton;
}

unsigned Event::getMouseX() const
{
    assert((m_type == Type::MouseMoved) | (m_type == Type::MouseScrolled));
    return m_mouseX;
}

unsigned Event::getMouseY() const
{
    assert((m_type == Type::MouseMoved) | (m_type == Type::MouseScrolled));
    return m_mouseY;
}

unsigned Event::getWindowID() const
{
    assert((m_type == Type::WindowClose) | (m_type == Type::WindowFocus) | (m_type == Type::WindowLostFocus) | (m_type == Type::WindowMoved) | (m_type == Type::WindowResize));
    return m_windowID;
}

void pushEvent(SDL_Event e)
{
    s_eventQueue.emplace(e);
}

void pushEvent(const Event& e)
{
    s_eventQueue.push(e);
}

void dispatchEvents(std::vector<std::reference_wrapper<EventHandler>>& handlers)
{
    while (!s_eventQueue.empty()) {
        for (EventHandler& handler : handlers) {
            handler.onEvent(s_eventQueue.front());
        }
        s_eventQueue.pop();
    }
}

} // namespace york
