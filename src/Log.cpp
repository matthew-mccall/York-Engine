#include <york/Log.hpp>

#include <spdlog/sinks/stdout_color_sinks.h>

namespace {

std::shared_ptr<spdlog::logger> s_coreLogger;
std::shared_ptr<spdlog::logger> s_clientLogger;

} // namespace

namespace york::log {

void init()
{
    spdlog::set_pattern("[%T.%f][%n][%^%8l%$][%7t] %v");

#ifdef NDEBUG
    spdlog::set_level(spdlog::level::err);
#else
    spdlog::set_level(spdlog::level::trace);
#endif

    s_coreLogger = spdlog::stdout_color_mt("Engine");
    s_clientLogger = spdlog::stderr_color_mt("Client");
}

std::shared_ptr<spdlog::logger> getCoreLogger()
{
    return s_coreLogger;
}

std::shared_ptr<spdlog::logger> getClientLogger()
{
    return s_clientLogger;
}

} // namespace york::log