//
// Created by Matthew McCall on 1/17/22.
//

#include "york/Log.hpp"

#include <SDL_timer.h>

#include <sstream>
#include <thread>

namespace york::log {

void commonEngine()
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    fmt::print("[{:>12.3f}][{:>16}][Engine][", static_cast<float>(SDL_GetTicks64()) / 1000.0f, ss.str());
}

void commonClient()
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    fmt::print("[{:>12.3f}][{:>16}][Client][", static_cast<float>(SDL_GetTicks64()) / 1000.0f, ss.str());
}

}