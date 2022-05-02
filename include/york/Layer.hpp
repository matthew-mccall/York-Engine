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

//
// Created by Matthew McCall on 12/19/21.
//

#ifndef YORK_LAYER_HPP
#define YORK_LAYER_HPP

#include "Event.hpp"
#include "Identifiable.hpp"

namespace york {

/**
 * A layer represents a modular part of your application
 *
 * Layers may be attached and detached on fly allowing for dynamic parts with your came. A layer is self-contained and should not depend on another layer.
 */
class Layer : public EventHandler {
public:
    Layer() = default;

    /**
     * A function that will be called when the layer is attached to the stack. Use this for initialization.
     */
    virtual void onAttach() { }

    /**
     * A function that will be called on every tick. Override this to implement your game logic.
     *
     * @param deltaTime The time since last tick.
     */
    virtual void onUpdate(float deltaTime) { }

    /**
     * A function that will be called on every frame. Override this to implement your rendering logic.
     */
    virtual void onRender() { }

    /**
     * A function that will be called when the layer is detached from the stack. Use this to clean up.
     */
    virtual void onDetach() { }

    [[nodiscard]] bool getExit() const;

    virtual ~Layer() = default;

protected:
    void requestExit();

private:
    bool m_exit = false;
};

}

#endif // YORK_LAYER_HPP
