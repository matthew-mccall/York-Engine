#if !defined(YORK_LOG_HPP)
#define YORK_LOG_HPP

#if !defined(NDEBUG)
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#else
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_ERROR
#endif

#include <memory>

#include <spdlog/spdlog.h>

#include "Async.hpp"

namespace york::log {

std::shared_ptr<spdlog::logger> getClientLogger();
std::shared_ptr<spdlog::logger> getCoreLogger();

void init();

#if !defined(NDEBUG)

#define YORK_TRACE(...)                                       \
    ::york::async::getExecutor().silent_async(                \
        [&]() {                                               \
            york::log::getClientLogger()->trace(__VA_ARGS__); \
        });

#define YORK_INFO(...)                                       \
    ::york::async::getExecutor().silent_async(               \
        [&]() {                                              \
            york::log::getClientLogger()->info(__VA_ARGS__); \
        });

#define YORK_DEBUG(...)                                       \
    ::york::async::getExecutor().silent_async(                \
        [&]() {                                               \
            york::log::getClientLogger()->debug(__VA_ARGS__); \
        });

#define YORK_WARN(...)                                       \
    ::york::async::getExecutor().silent_async(               \
        [&]() {                                              \
            york::log::getClientLogger()->warn(__VA_ARGS__); \
        });

#define YORK_ERROR(...)                                       \
    ::york::async::getExecutor().silent_async(                \
        [&]() {                                               \
            york::log::getClientLogger()->error(__VA_ARGS__); \
        });

#define YORK_CRITICAL(...)                                       \
    ::york::async::getExecutor().silent_async(                   \
        [&]() {                                                  \
            york::log::getClientLogger()->critical(__VA_ARGS__); \
        });

#define YORK_CORE_TRACE(...)                                \
    ::york::async::getExecutor().silent_async(              \
        [&]() {                                             \
            york::log::getCoreLogger()->trace(__VA_ARGS__); \
        });

#define YORK_CORE_INFO(...)                                \
    ::york::async::getExecutor().silent_async(             \
        [&]() {                                            \
            york::log::getCoreLogger()->info(__VA_ARGS__); \
        });

#define YORK_CORE_DEBUG(...)                                \
    ::york::async::getExecutor().silent_async(              \
        [&]() {                                             \
            york::log::getCoreLogger()->debug(__VA_ARGS__); \
        });

#define YORK_CORE_WARN(...)                                \
    ::york::async::getExecutor().silent_async(             \
        [&]() {                                            \
            york::log::getCoreLogger()->warn(__VA_ARGS__); \
        });

#define YORK_CORE_ERROR(...)                                \
    ::york::async::getExecutor().silent_async(              \
        [&]() {                                             \
            york::log::getCoreLogger()->error(__VA_ARGS__); \
        });

#define YORK_CORE_CRITICAL(...)                                \
    ::york::async::getExecutor().silent_async(                 \
        [&]() {                                                \
            york::log::getCoreLogger()->critical(__VA_ARGS__); \
        });

#else

#define YORK_TRACE
#define YORK_INFO
#define YORK_DEBUG
#define YORK_WARN
#define YORK_ERROR
#define YORK_CRITICAL

#define YORK_CORE_TRACE
#define YORK_CORE_INFO
#define YORK_CORE_DEBUG
#define YORK_CORE_WARN
#define YORK_CORE_ERROR
#define YORK_CORE_CRITICAL

#endif // NDEBUG

} // namespace york::log

#endif // YORK_LOG_HPP
