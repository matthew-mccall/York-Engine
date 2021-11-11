#if !defined(YORK_ENTRY_HPP)
#define YORK_ENTRY_HPP

#include <cstdlib>

#include <SDL.h>

#include "Application.hpp"
#include "Async.hpp"
#include "Event.hpp"
#include "Log.hpp"
#include "Timer.hpp"

extern york::Application* york::createApplication();

int main()
{
    york::Timer timer;

    york::log::init();

    york::Application* app = york::createApplication();

    york::log::core::info("Initalization took {} seconds!", timer.getTime());

    SDL_Event event;

    while (!app->getExit()) {
        timer.reset();

        while (SDL_PollEvent(&event)) {
            york::pushEvent(event);
        }

        york::Event tickEvent(york::Event::Type::AppTick);
        tickEvent.m_tickTime = timer.getTime();

        york::Event renderEvent(york::Event::Type::AppRender);

        york::pushEvent(tickEvent);
        york::pushEvent(renderEvent);

        york::dispatchEvents();
    }

    timer.reset();

    york::async::getExecutor().wait_for_all();

    delete app;

    SDL_Quit();

    york::log::core::info("Shutdown took {} seconds!", timer.getTime());
    return EXIT_SUCCESS;
}

#endif // YORK_ENTRY_HPP
