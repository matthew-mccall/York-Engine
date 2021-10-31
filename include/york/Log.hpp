#if !defined(YORK_LOG_HPP)
#define YORK_LOG_HPP

#include <memory>

#include <spdlog/spdlog.h>

#include "Async.hpp"

namespace york::log {

std::shared_ptr<spdlog::logger> getCoreLogger();
std::shared_ptr<spdlog::logger> getClientLogger();

void init();

template <typename... ArgsT>
void trace(ArgsT&&... args)
{
    async::dispatch([](auto args) {
        getClientLogger()->trace(args);
    },
        std::forward<ArgsT>(args)...);
}

template <typename... ArgsT>
void info(ArgsT&&... args)
{
    async::dispatch([](auto args) {
        getClientLogger()->info(args);
    },
        std::forward<ArgsT>(args)...);
}

template <typename... ArgsT>
void debug(ArgsT&&... args)
{
    async::dispatch([](auto args) {
        getClientLogger()->debug(args);
    },
        std::forward<ArgsT>(args)...);
}

template <typename... ArgsT>
void warn(ArgsT&&... args)
{
    async::dispatch([](auto args) {
        getClientLogger()->warn(args);
    },
        std::forward<ArgsT>(args)...);
}

template <typename... ArgsT>
void error(ArgsT&&... args)
{
    async::dispatch([](auto args) {
        getCoreLogger()->error(args);
    },
        std::forward<ArgsT>(args)...);
}

template <typename... ArgsT>
void critical(ArgsT&&... args)
{
    async::dispatch([](auto args) {
        getCoreLogger()->critical(args);
    },
        std::forward<ArgsT>(args)...);
}

namespace core {

    template <typename... ArgsT>
    void trace(ArgsT&&... args)
    {
        async::dispatch([](auto args) {
            getCoreLogger()->trace(args);
        },
            std::forward<ArgsT>(args)...);
    }

    template <typename... ArgsT>
    void info(ArgsT&&... args)
    {
        async::dispatch([](auto args) {
            getCoreLogger()->info(args);
        },
            std::forward<ArgsT>(args)...);
    }

    template <typename... ArgsT>
    void debug(ArgsT&&... args)
    {
        async::dispatch([](auto args) {
            getCoreLogger()->debug(args);
        },
            std::forward<ArgsT>(args)...);
    }

    template <typename... ArgsT>
    void warn(ArgsT&&... args)
    {
        async::dispatch([](auto args) {
            getCoreLogger()->warn(args);
        },
            std::forward<ArgsT>(args)...);
    }

    template <typename... ArgsT>
    void error(ArgsT&&... args)
    {
        async::dispatch([](auto args) {
            getCoreLogger()->error(args);
        },
            std::forward<ArgsT>(args)...);
    }

    template <typename... ArgsT>
    void critical(ArgsT&&... args)
    {
        async::dispatch([](auto args) {
            getCoreLogger()->critical(args);
        },
            std::forward<ArgsT>(args)...);
    }

} // namespace core

} // namespace york::log

#endif // YORK_LOG_HPP
