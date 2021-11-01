#if !defined(YORK_LOG_HPP)
#define YORK_LOG_HPP

#include <memory>

#include <spdlog/spdlog.h>

#include "Async.hpp"

namespace york::log {

std::shared_ptr<spdlog::logger> getCoreLogger();
std::shared_ptr<spdlog::logger> getClientLogger();

void init();

/**
 * @brief Logs trace messages.
 *
 * @tparam ArgsT The parameter pack
 * @param args See https://fmt.dev/latest/syntax.html#formatexamples
 */
template <typename... ArgsT>
void trace(ArgsT&&... args)
{
    async::nrDispatch(
        []<typename... LArgsT>(LArgsT && ... largs) {
            getClientLogger()->trace(std::forward<LArgsT>(largs)...);
        },
        std::forward<ArgsT>(args)...);
}

/**
 * @brief Logs information messages.
 *
 * @tparam ArgsT The parameter pack
 * @param args See https://fmt.dev/latest/syntax.html#formatexamples
 */
template <typename... ArgsT>
void info(ArgsT&&... args)
{
    async::nrDispatch(
        []<typename... LArgsT>(LArgsT && ... largs) {
            getClientLogger()->info(std::forward<LArgsT>(largs)...);
        },
        std::forward<ArgsT>(args)...);
}

/**
 * @brief Logs debug messages.
 *
 * @tparam ArgsT The parameter pack
 * @param args See https://fmt.dev/latest/syntax.html#formatexamples
 */
template <typename... ArgsT>
void debug(ArgsT&&... args)
{
    async::nrDispatch(
        []<typename... LArgsT>(LArgsT && ... largs) {
            getClientLogger()->debug(std::forward<LArgsT>(largs)...);
        },
        std::forward<ArgsT>(args)...);
}

/**
 * @brief Logs warning messages.
 *
 * @tparam ArgsT The parameter pack
 * @param args See https://fmt.dev/latest/syntax.html#formatexamples
 */
template <typename... ArgsT>
void warn(ArgsT&&... args)
{
    async::nrDispatch(
        []<typename... LArgsT>(LArgsT && ... largs) {
            getClientLogger()->warn(std::forward<LArgsT>(largs)...);
        },
        std::forward<ArgsT>(args)...);
}

/**
 * @brief Logs non-fatal, error messages.
 *
 * @tparam ArgsT The parameter pack
 * @param args See https://fmt.dev/latest/syntax.html#formatexamples
 */
template <typename... ArgsT>
void error(ArgsT&&... args)
{
    async::nrDispatch(
        []<typename... LArgsT>(LArgsT && ... largs) {
            getClientLogger()->error(std::forward<LArgsT>(largs)...);
        },
        std::forward<ArgsT>(args)...);
}

/**
 * @brief Logs fatal error messages.
 *
 * @tparam ArgsT The parameter pack
 * @param args See https://fmt.dev/latest/syntax.html#formatexamples
 */
template <typename... ArgsT>
void critical(ArgsT&&... args)
{
    async::nrDispatch(
        []<typename... LArgsT>(LArgsT && ... largs) {
            getClientLogger()->critical(std::forward<LArgsT>(largs)...);
        },
        std::forward<ArgsT>(args)...);
}

namespace core {

    /**
     * @brief Logs trace messages. For engine use only.
     *
     * @tparam ArgsT The parameter pack
     * @param args See https://fmt.dev/latest/syntax.html#formatexamples
     */
    template <typename... ArgsT>
    void trace(ArgsT&&... args)
    {
        async::nrDispatch(
            []<typename... LArgsT>(LArgsT && ... largs) {
                getCoreLogger()->trace(std::forward<LArgsT>(largs)...);
            },
            std::forward<ArgsT>(args)...);
    }

    /**
     * @brief Logs information messages. For engine use only.
     *
     * @tparam ArgsT The parameter pack
     * @param args See https://fmt.dev/latest/syntax.html#formatexamples
     */
    template <typename... ArgsT>
    void info(ArgsT&&... args)
    {
        async::nrDispatch(
            []<typename... LArgsT>(LArgsT && ... largs) {
                getCoreLogger()->info(std::forward<LArgsT>(largs)...);
            },
            std::forward<ArgsT>(args)...);
    }

    /**
     * @brief Logs debug messages. For engine use only.
     *
     * @tparam ArgsT The parameter pack
     * @param args See https://fmt.dev/latest/syntax.html#formatexamples
     */
    template <typename... ArgsT>
    void debug(ArgsT&&... args)
    {
        async::nrDispatch(
            []<typename... LArgsT>(LArgsT && ... largs) {
                getCoreLogger()->debug(std::forward<LArgsT>(largs)...);
            },
            std::forward<ArgsT>(args)...);
    }

    /**
     * @brief Logs warning messages. For engine use only.
     *
     * @tparam ArgsT The parameter pack
     * @param args See https://fmt.dev/latest/syntax.html#formatexamples
     */
    template <typename... ArgsT>
    void warn(ArgsT&&... args)
    {
        async::nrDispatch(
            []<typename... LArgsT>(LArgsT && ... largs) {
                getCoreLogger()->warn(std::forward<LArgsT>(largs)...);
            },
            std::forward<ArgsT>(args)...);
    }

    /**
     * @brief Logs non-fatal, error messages. For engine use only
     *
     * @tparam ArgsT The parameter pack
     * @param args See https://fmt.dev/latest/syntax.html#formatexamples
     */
    template <typename... ArgsT>
    void error(ArgsT&&... args)
    {
        async::nrDispatch(
            []<typename... LArgsT>(LArgsT && ... largs) {
                getCoreLogger()->error(std::forward<LArgsT>(largs)...);
            },
            std::forward<ArgsT>(args)...);
    }

    /**
     * @brief Logs fatal error messages. For engine use only
     *
     * @tparam ArgsT The parameter pack
     * @param args See https://fmt.dev/latest/syntax.html#formatexamples
     */
    template <typename... ArgsT>
    void critical(ArgsT&&... args)
    {
        async::nrDispatch(
            []<typename... LArgsT>(LArgsT && ... largs) {
                getCoreLogger()->critical(std::forward<LArgsT>(largs)...);
            },
            std::forward<ArgsT>(args)...);
    }

} // namespace core

} // namespace york::log

#endif // YORK_LOG_HPP
