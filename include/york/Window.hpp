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

#if !defined(YORK_VULKAN_WINDOW_HPP)
#define YORK_VULKAN_WINDOW_HPP

#include <string>

#include <SDL_video.h>

namespace york {

/**
 * A graphical window to which we can render to.
 */
class Window {
public:
    /**
     * Creates a Window.
     *
     * @param name The name of the window to go in the titlebar.
     * @param width The initial width of the window.
     * @param height The initial height of the window.
     */
    explicit Window(std::string name, unsigned width = 800, unsigned height = 600);

    /**
     * Gets the name of the window.
     *
     * @return  The name of the window.
     */
    [[nodiscard]] std::string getName() const;

    /**
     * Gets the identification number of the window.
     *
     * @return The ID number of the window.
     */
    unsigned getWindowID();

    bool isOpen();
    void close();
    SDL_Window* getHandle();
    virtual ~Window();

private:
    unsigned m_width, m_height;
    std::string m_name;
    SDL_Window* m_handle = nullptr;
};
} // namespace york::graphics

#endif
