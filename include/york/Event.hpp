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

#if !defined(YORK_EVENT_HPP)
#define YORK_EVENT_HPP

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

void pushEvent(Event e);

/**
 * Override this to handle events.
 */
class EventHandler : public Identifiable {
public:
    explicit EventHandler();
    virtual void onEvent(Event& e) = 0;
    virtual ~EventHandler();
};

void broadcastEvent(Event e);
void flushEventQueue();

} // namespace york

#endif // YORK_EVENT_HPP
