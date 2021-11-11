#include <cassert>
#include <queue>
#include <vector>

#include "SDL_events.h"

#include "york/Event.hpp"

namespace {

std::queue<york::Event> s_eventQueue;
std::vector<york::EventHandler*> s_eventHandlers;

} // namespace

namespace york {

Event::Event(SDL_Event e)
{
    switch (e.type) {

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

Event::Event(Type type)
    : m_type(type)
{
}

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

float Event::getTickTime() const
{
    assert(m_type == Type::AppTick);
    return m_tickTime;
}

unsigned Event::getWindowID() const
{
    assert((m_type == Type::WindowClose) | (m_type == Type::WindowFocus) | (m_type == Type::WindowLostFocus) | (m_type == Type::WindowMoved) | (m_type == Type::WindowResize));
    return m_windowID;
}

EventHandler::EventHandler()
{
    s_eventHandlers.push_back(this);
}

EventHandler::~EventHandler()
{
    for (auto i = s_eventHandlers.begin(); i != s_eventHandlers.end(); i++) {
        if (this == *i) {
            s_eventHandlers.erase(i);
            break;
        }
    }
}

void pushEvent(SDL_Event e)
{
    s_eventQueue.emplace(e);
}

void pushEvent(Event e)
{
    s_eventQueue.push(e);
}

void dispatchEvents()
{
    while (!s_eventQueue.empty()) {
        for (EventHandler* handler : s_eventHandlers) {
            handler->onEvent(s_eventQueue.front());
        }
        s_eventQueue.pop();
    }
}

} // namespace york
