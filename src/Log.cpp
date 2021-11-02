#include <york/Log.hpp>

#include <spdlog/sinks/stdout_color_sinks.h>

namespace {

static std::shared_ptr<spdlog::logger> s_coreLogger;
static std::shared_ptr<spdlog::logger> s_clientLogger;

} // namespace

namespace york::log {

void init()
{
    spdlog::set_pattern("[%T.%f][%n][%^%8l%$][%7t] %v");
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