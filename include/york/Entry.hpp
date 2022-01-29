#if !defined(YORK_ENTRY_HPP)
#define YORK_ENTRY_HPP

#include <cstdlib>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <SDL.h>
#include <xercesc/util/PlatformUtils.hpp>

#include "Application.hpp"
#include "Async.hpp"
#include "Event.hpp"
#include "Exit.hpp"
#include "Log.hpp"
#include "Timer.hpp"

int main()
{
    york::Timer timer;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        YORK_CORE_ERROR(SDL_GetError());
        return EXIT_FAILURE;
    }

    try {
        xercesc::XMLPlatformUtils::Initialize();
    }
    catch (const xercesc::XMLException& e) {
        YORK_CORE_CRITICAL("Failed to load XML library!")
        return EXIT_FAILURE;
    }

    curlpp::initialize();

    york::Application* app = york::createApplication();

    YORK_CORE_INFO("Initialization took {} seconds!", timer.getTime());

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
        YORK_CORE_CRITICAL(e.what());
    }

    york::async::getExecutor().wait_for_all();

    delete app;

    xercesc::XMLPlatformUtils::Terminate();
    curlpp::terminate();
    SDL_Quit();

    YORK_CORE_INFO("Shutdown successfully!");
    return EXIT_SUCCESS;
}

#endif // YORK_ENTRY_HPP
