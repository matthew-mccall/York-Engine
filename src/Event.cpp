/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2022 Matthew McCall
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cassert>
#include <vector>

#include <boost/circular_buffer.hpp>
#include <SDL_events.h>

#include "york/Event.hpp"
#include "york/Registry.hpp"

namespace {

boost::circular_buffer<york::Event> s_eventQueue { 256 };

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

    case SDL_QUIT:
        m_type = Type::AppClosing;
        break ;

    case SDL_WINDOWEVENT:

        switch (e.window.event) {

        case SDL_WINDOWEVENT_CLOSE:
            m_type = Type::WindowClose;
            m_windowID = e.window.windowID;
            break;

        case SDL_WINDOWEVENT_SIZE_CHANGED:
            m_type = Type::WindowResize;
            m_windowID = e.window.windowID;
            break;

        case SDL_WINDOWEVENT_MOVED:
            m_type = Type::WindowMoved;
            m_windowID = e.window.windowID;
            break;

        default:
            m_type = Type::None;
            break;
        }

        break;

    default:
        m_type = Type::None;
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

void pushEvent(const Event e)
{
    s_eventQueue.push_back(e);
}

void broadcastEvent(Event e)
{
    for (EventHandler& handler : Registry::getInstance().m_eventHandlers) {
        handler.onEvent(e);
    }
}

void flushEventQueue()
{
    while (!s_eventQueue.empty()) {
        broadcastEvent(s_eventQueue.front());
        s_eventQueue.pop_front();
    }
}

EventHandler::EventHandler()
{
    Registry::getInstance().m_eventHandlers.emplace_back(*this);
}

EventHandler::~EventHandler()
{
    auto& handlers = Registry::getInstance().m_eventHandlers;

    for (auto i = handlers.begin(); i != handlers.end(); i++) {
        if (*this == i->get()) {
            handlers.erase(i);
            return;
        }
    }
}

} // namespace york
