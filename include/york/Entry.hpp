#if !defined(YORK_ENTRY_HPP)
#define YORK_ENTRY_HPP

#include <cstdlib>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <SDL.h>

#include "Application.hpp"
#include "Async.hpp"
#include "Event.hpp"
#include "Exit.hpp"
#include "Log.hpp"
#include "Timer.hpp"

int main()
{
    york::Timer timer;

    curlpp::initialize();

    york::Application* app = york::createApplication();

    york::log::core::info("Initialization took {} seconds!", timer.getTime());

    SDL_Event event;

    timer.reset();

    try {
        while (!york::getExit()) {
            while (SDL_PollEvent(&event)) {
                york::pushEvent(event);
            }

            york::dispatchEvents();

            for (york::Layer& layer : app->getLayerStack()) {
                layer.onUpdate(timer.reset().getTime());
            }
        }
    } catch (std::exception& e) {
        york::log::core::critical(e.what());
    }

    timer.reset();

    york::async::getExecutor().wait_for_all();

    delete app;

    curlpp::terminate();
    SDL_Quit();

    york::log::core::info("Shutdown took {} seconds!", timer.getTime());
    return EXIT_SUCCESS;
}

#endif // YORK_ENTRY_HPP
