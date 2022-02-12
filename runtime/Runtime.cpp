#include <cstdlib>
#include <vector>

#include <SDL.h>
#include <xercesc/util/PlatformUtils.hpp>

#include "curlpp/cURLpp.hpp"

#include "york/Async.hpp"
#include "york/Event.hpp"
#include "york/Config.hpp"
#include "york/LayerStack.hpp"
#include "york/Log.hpp"
#include "york/Timer.hpp"
#include "york/XML/Document.hpp"

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

    auto* clientXML = new york::xml::Document { "Client.xml" };
    york::xml::Tag& clientTag = *clientXML->getRootTag();
    york::xml::Tag* layersTag;

    std::vector<LayerLoader> layerLoaders;

    for (auto& tag : clientTag) {
        if (tag.getName() == "Layers") {
            layersTag = &tag;
        }
    }

    for (auto& tag: *layersTag) {
        if (tag.getName() == "Layer") {
            std::string layerName, createFunctionName;

            for (auto& layerChildTag: tag) {
                if (layerChildTag.getName() == "Name") {
                    layerName = layerChildTag.getValue();
                }
                if (layerChildTag.getName() == "CreateFunction") {
                    createFunctionName = layerChildTag.getValue();
                }
            }

            if (!createFunctionName.empty()) {
                layerLoaders.emplace_back(layerName, createFunctionName);
                continue;
            }

            layerLoaders.emplace_back(layerName);

        }
    }


    york::LayerStack layerStack;
    std::vector<std::reference_wrapper<york::EventHandler>> eventHandlers;

    for (auto& layerLoader: layerLoaders) {
        layerStack.pushLayer(*layerLoader);
        eventHandlers.emplace_back(*layerLoader);
    }

    curlpp::initialize();

    YORK_CORE_INFO("Initialization took {} seconds on {}!", timer.getTime(), YORK_PLATFORM);

    SDL_Event event;

    timer.reset();

    try {
        while (!layerStack.empty()) {
            while (SDL_PollEvent(&event)) {
                york::pushEvent(event);
                york::dispatchEvents(eventHandlers);
            }

            for (york::Layer& layer : layerStack) {

                if (layer.getExit()) {
                    layerStack.popLayer(layer);
                    continue;
                }

                layer.onUpdate(timer.reset().getTime());
            }
        }
    } catch (std::exception& e) {
        YORK_CORE_CRITICAL(e.what());
    }

    layerLoaders.clear();

    york::async::getExecutor().wait_for_all();

    curlpp::terminate();

    delete clientXML;
    xercesc::XMLPlatformUtils::Terminate();

    SDL_Quit();

    YORK_CORE_INFO("Shutdown successfully!");
    return EXIT_SUCCESS;
}