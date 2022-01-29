#if !defined(YORK_TIMER_HPP)
#define YORK_TIMER_HPP

#include <chrono>

namespace york {

class Timer {
private:
    std::uint64_t m_startTime;

public:
    Timer();
    [[nodiscard]] float getTime() const;
    Timer& reset();
};

} // namespace york

#endif // YORK_TIMER_HPP
