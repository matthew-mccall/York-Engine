#include <york/Timer.hpp>

namespace york {

Timer::Timer()
{
    reset();
}

Timer& Timer::reset()
{
    m_startTime = std::chrono::steady_clock::now();
    return *this;
}

float Timer::getTime()
{
    return std::chrono::duration<float>(std::chrono::steady_clock::now() - m_startTime).count();
}

} // namespace york
