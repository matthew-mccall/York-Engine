#include <cassert>
#include <queue>
#include <utility>
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
            m_data = WindowData { e.window.windowID };
            break;

        default:
            break;
        }

        break;

    default:
        break;
    }
}

Event::Event(Event::KeyData data, Event::Type type)
    : m_type(type)
{
    assert((m_type == Type::KeyPressed) | (m_type == Type::KeyReleased) | (m_type == Type::KeyTyped));
    m_data.emplace<0>(data);
}

Event::Event(Event::MouseButtonData data, Event::Type type)
    : m_type(type)
{
    assert((m_type == Type::MouseButtonPressed) | (m_type == Type::MouseButtonReleased));
    m_data.emplace<1>(data);
}

Event::Event(Event::MouseData data, Event::Type type)
    : m_type(type)
{
    assert((m_type == Type::MouseMoved) | (m_type == Type::MouseScrolled));
    m_data.emplace<2>(data);
}

Event::Event(Event::WindowData data, Event::Type type)
    : m_type(type)
{
    assert((m_type == Type::WindowClose) | (m_type == Type::WindowFocus) | (m_type == Type::WindowLostFocus) | (m_type == Type::WindowMoved) | (m_type == Type::WindowResize));
    m_data.emplace<3>(data);
}

Event::Type Event::getType() const
{
    return m_type;
}

KeyCode Event::getKeyCode() const
{
    assert((m_type == Type::KeyPressed) | (m_type == Type::KeyReleased) | (m_type == Type::KeyTyped));
    assert(std::holds_alternative<KeyData>(m_data));
    return std::get<KeyData>(m_data).m_keyCode;
}

unsigned Event::getMouseButton() const
{
    assert((m_type == Type::MouseButtonPressed) | (m_type == Type::MouseButtonReleased));
    assert(std::holds_alternative<MouseButtonData>(m_data));
    return std::get<MouseButtonData>(m_data).m_mouseButton;
}

unsigned Event::getMouseX() const
{
    assert((m_type == Type::MouseMoved) | (m_type == Type::MouseScrolled));
    assert(std::holds_alternative<MouseData>(m_data));
    return std::get<MouseData>(m_data).m_mouseX;
}

unsigned Event::getMouseY() const
{
    assert((m_type == Type::MouseMoved) | (m_type == Type::MouseScrolled));
    assert(std::holds_alternative<MouseData>(m_data));
    return std::get<MouseData>(m_data).m_mouseY;
}

unsigned Event::getWindowID() const
{
    assert((m_type == Type::WindowClose) | (m_type == Type::WindowFocus) | (m_type == Type::WindowLostFocus) | (m_type == Type::WindowMoved) | (m_type == Type::WindowResize));
    assert(std::holds_alternative<WindowData>(m_data));
    return std::get<WindowData>(m_data).m_windowID;
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

void pushEvent(const Event& e)
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
