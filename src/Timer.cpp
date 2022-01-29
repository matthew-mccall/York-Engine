#include <SDL_timer.h>

#include <york/Timer.hpp>

namespace york {

Timer::Timer()
{
    reset();
}

Timer& Timer::reset()
{
    m_startTime = SDL_GetTicks64();
    return *this;
}

float Timer::getTime() const
{
    return static_cast<float >((SDL_GetTicks64() - m_startTime)) / 1000.0f;
}

} // namespace york
