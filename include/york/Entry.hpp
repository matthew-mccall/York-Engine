#if !defined(YORK_ENTRY_HPP)
#define YORK_ENTRY_HPP

#include <cstdlib>

#include "Application.hpp"
#include "Async.hpp"
#include "Log.hpp"
#include "Timer.hpp"

extern york::Application* york::createApplication();

int main()
{
    york::Timer timer;

    york::log::init();

    york::Application* app = york::createApplication();

    york::log::core::info("Initalization took {} seconds!", timer.getTime());

    while (!app->getExit()) {
        app->onUpdate(0);
        app->onRender();
    }

    timer.reset();

    york::async::getExecutor().wait_for_all();

    delete app;

    york::log::core::info("Shutdown took {} seconds!", timer.getTime());
    york::async::getExecutor().wait_for_all();

    return EXIT_SUCCESS;
}

#endif // YORK_ENTRY_HPP
