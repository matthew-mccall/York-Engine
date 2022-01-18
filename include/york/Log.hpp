#if !defined(YORK_LOG_HPP)
#define YORK_LOG_HPP

#include <memory>

#include "fmt/core.h"
#include "fmt/color.h"

namespace york::log {

void commonEngine();
void commonClient();

template <typename... Ts>
void trace(Ts&&... args)
{
    commonClient();
    fmt::print(fmt::fg(fmt::color::gray), "{:>8}", "trace");
    fmt::print("] ");
    fmt::print(std::forward<Ts>(args)...);
    fmt::print("\n");
}

template <typename... Ts>
void info(Ts&&... args)
{
    commonClient();
    fmt::print(fmt::fg(fmt::color::light_gray), "{:>8}", "info");
    fmt::print("] ");
    fmt::print(std::forward<Ts>(args)...);
    fmt::print("\n");
}

template <typename... Ts>
void debug(Ts&&... args)
{
    commonClient();
    fmt::print(fmt::fg(fmt::color::cyan), "{:>8}", "debug");
    fmt::print("] ");
    fmt::print(std::forward<Ts>(args)...);
    fmt::print("\n");
}

template <typename... Ts>
void warn(Ts&&... args)
{
    commonClient();
    fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::yellow), "{:>8}", "warning");
    fmt::print("] ");
    fmt::print(std::forward<Ts>(args)...);
    fmt::print("\n");
}

template <typename... Ts>
void error(Ts&&... args)
{
    commonClient();
    fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::orange), "{:>8}", "error");
    fmt::print("] ");
    fmt::print(std::forward<Ts>(args)...);
    fmt::print("\n");
}

template <typename... Ts>
void critical(Ts&&... args)
{
    commonClient();
    fmt::print(fmt::emphasis::blink | fmt::emphasis::bold | fmt::fg(fmt::color::red), "{:>8}", "critical");
    fmt::print("] ");
    fmt::print(std::forward<Ts>(args)...);
    fmt::print("\n");
}

namespace core {

    template <typename... Ts>
    void trace(Ts&&... args)
    {
        commonEngine();
        fmt::print(fmt::fg(fmt::color::gray), "{:>8}", "trace");
        fmt::print("] ");
        fmt::print(std::forward<Ts>(args)...);
        fmt::print("\n");
    }

    template <typename... Ts>
    void info(Ts&&... args)
    {
        commonEngine();
        fmt::print(fmt::fg(fmt::color::light_gray), "{:>8}", "info");
        fmt::print("] ");
        fmt::print(std::forward<Ts>(args)...);
        fmt::print("\n");
    }

    template <typename... Ts>
    void debug(Ts&&... args)
    {
        commonEngine();
        fmt::print(fmt::fg(fmt::color::cyan), "{:>8}", "debug");
        fmt::print("] ");
        fmt::print(std::forward<Ts>(args)...);
        fmt::print("\n");
    }

    template <typename... Ts>
    void warn(Ts&&... args)
    {
        commonEngine();
        fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::yellow), "{:>8}", "warning");
        fmt::print("] ");
        fmt::print(std::forward<Ts>(args)...);
        fmt::print("\n");
    }

    template <typename... Ts>
    void error(Ts&&... args)
    {
        commonEngine();
        fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::orange), "{:>8}", "error");
        fmt::print("] ");
        fmt::print(std::forward<Ts>(args)...);
        fmt::print("\n");
    }

    template <typename... Ts>
    void critical(Ts&&... args)
    {
        commonEngine();
        fmt::print(fmt::emphasis::blink | fmt::emphasis::bold | fmt::fg(fmt::color::red), "{:>8}", "critical");
        fmt::print("] ");
        fmt::print(std::forward<Ts>(args)...);
        fmt::print("\n");
    }

} // namespace core

} // namespace york::log

#define YORK_TRACE(...) york::log::trace(__VA_ARGS__);
#define YORK_INFO(...) york::log::info(__VA_ARGS__);
#define YORK_DEBUG(...) york::log::debug(__VA_ARGS__);
#define YORK_WARN(...) york::log::warn(__VA_ARGS__);
#define YORK_ERROR(...) york::log::error(__VA_ARGS__);
#define YORK_CRITICAL(...) york::log::critical(__VA_ARGS__);

#define YORK_CORE_TRACE(...) york::log::core::trace(__VA_ARGS__);
#define YORK_CORE_INFO(...) york::log::core::info(__VA_ARGS__);
#define YORK_CORE_DEBUG(...) york::log::core::debug(__VA_ARGS__);
#define YORK_CORE_WARN(...) york::log::core::warn(__VA_ARGS__);
#define YORK_CORE_ERROR(...) york::log::core::error(__VA_ARGS__);
#define YORK_CORE_CRITICAL(...) york::log::core::critical(__VA_ARGS__);

#ifdef NDEBUG

#define YORK_TRACE
#define YORK_INFO
#define YORK_DEBUG

#define YORK_CORE_TRACE
#define YORK_CORE_INFO
#define YORK_CORE_DEBUG

#endif

#endif // YORK_LOG_HPP
