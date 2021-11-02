#if !defined(YORK_TIMER_HPP)
#define YORK_TIMER_HPP

#include <chrono>

namespace york {

class Timer {
private:
    std::chrono::steady_clock::time_point m_startTime;

public:
    Timer();
    float getTime();
    void reset();
};

} // namespace york

#endif // YORK_TIMER_HPP
