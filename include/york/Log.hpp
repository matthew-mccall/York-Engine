#if !defined(YORK_LOG_HPP)
#define YORK_LOG_HPP

#include <memory>

#include <spdlog/spdlog.h>

namespace {

std::shared_ptr<spdlog::logger> getCoreLogger();
std::shared_ptr<spdlog::logger> getClientLogger();

} // namespace

namespace york::log {

void init();

template <typename... Ts>
void trace(Ts&&... args)
{
    getClientLogger()->trace(std::forward<Ts>(args)...);
}

template <typename... Ts>
void info(Ts&&... args)
{
    getClientLogger()->info(std::forward<Ts>(args)...);
}

template <typename... Ts>
void debug(Ts&&... args)
{
    getClientLogger()->debug(std::forward<Ts>(args)...);
}

template <typename... Ts>
void warn(Ts&&... args)
{
    getClientLogger()->warn(std::forward<Ts>(args)...);
}

template <typename... Ts>
void error(Ts&&... args)
{
    getClientLogger()->error(std::forward<Ts>(args)...);
}

template <typename... Ts>
void critical(Ts&&... args)
{
    getClientLogger()->critical(std::forward<Ts>(args)...);
}

namespace core {

    template <typename... Ts>
    void trace(Ts&&... args)
    {
        getCoreLogger()->trace(std::forward<Ts>(args)...);
    }

    template <typename... Ts>
    void info(Ts&&... args)
    {
        getCoreLogger()->info(std::forward<Ts>(args)...);
    }

    template <typename... Ts>
    void debug(Ts&&... args)
    {
        getCoreLogger()->debug(std::forward<Ts>(args)...);
    }

    template <typename... Ts>
    void warn(Ts&&... args)
    {
        getCoreLogger()->warn(std::forward<Ts>(args)...);
    }

    template <typename... Ts>
    void error(Ts&&... args)
    {
        getCoreLogger()->error(std::forward<Ts>(args)...);
    }

    template <typename... Ts>
    void critical(Ts&&... args)
    {
        getCoreLogger()->critical(std::forward<Ts>(args)...);
    }

} // namespace core

} // namespace york::log

#endif // YORK_LOG_HPP
