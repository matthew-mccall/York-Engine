#if !defined(YORK_ENTRY_HPP)
#define YORK_ENTRY_HPP

#include <cstdlib>

#include "Application.hpp"
#include "Async.hpp"
#include "Log.hpp"

extern york::Application* york::createApplication();

int main()
{

    york::log::init();

    york::Application* app = york::createApplication();

    while (!app->getExit()) {
        app->onUpdate(0);
        app->onRender();
    }

    york::async::getExecutor().wait_for_all();

    delete app;

    return EXIT_SUCCESS;
}

#endif // YORK_ENTRY_HPP
