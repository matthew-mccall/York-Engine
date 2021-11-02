#include <york/Timer.hpp>

namespace york {

Timer::Timer()
{
    reset();
}

void Timer::reset()
{
    m_startTime = std::chrono::steady_clock::now();
}

float Timer::getTime()
{
    return std::chrono::duration<float>(std::chrono::steady_clock::now() - m_startTime).count();
}

} // namespace york
