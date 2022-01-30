#include <cstdlib>
#include <vector>

#include <SDL.h>
#include <xercesc/util/PlatformUtils.hpp>

#include "curlpp/cURLpp.hpp"

#include "york/Async.hpp"
#include "york/Event.hpp"
#include "york/Exit.hpp"
#include "york/LayerStack.hpp"
#include "york/Log.hpp"
#include "york/Timer.hpp"

#include "LayerLoader.hpp"

int main()
{
    york::Timer timer;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        YORK_CORE_ERROR(SDL_GetError());
        return EXIT_FAILURE;
    }

    try {
        xercesc::XMLPlatformUtils::Initialize();
    } catch (const xercesc::XMLException& e) {
        YORK_CORE_CRITICAL("Failed to load XML library!")
        return EXIT_FAILURE;
    }

    york::LayerStack layerStack;
    std::vector<std::reference_wrapper<york::EventHandler>> eventHandlers;

    LayerLoader newlayer { "libYorkTest.dylib" };
    layerStack.pushLayer(*newlayer);
    eventHandlers.emplace_back(*newlayer);

    curlpp::initialize();

    YORK_CORE_INFO("Initialization took {} seconds!", timer.getTime());

    SDL_Event event;

    timer.reset();

    try {
        while (!newlayer->getExit()) {
            while (SDL_PollEvent(&event)) {
                york::pushEvent(event);
                york::dispatchEvents(eventHandlers);
            }

            for (york::Layer& layer : layerStack) {
                layer.onUpdate(timer.reset().getTime());
            }
        }
    } catch (std::exception& e) {
        YORK_CORE_CRITICAL(e.what());
    }

    layerStack.popLayer(*newlayer);

    york::async::getExecutor().wait_for_all();

    xercesc::XMLPlatformUtils::Terminate();
    curlpp::terminate();
    SDL_Quit();

    YORK_CORE_INFO("Shutdown successfully!");
    return EXIT_SUCCESS;
}