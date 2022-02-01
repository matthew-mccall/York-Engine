#include <cstdlib>
#include <vector>

#include <SDL.h>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>

#include "curlpp/cURLpp.hpp"

#include "york/Async.hpp"
#include "york/Event.hpp"
#include "york/Config.hpp"
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


    auto* domParser = new xercesc::XercesDOMParser();
    domParser->setValidationScheme(xercesc::XercesDOMParser::Val_Always);
    domParser->setDoNamespaces(true);

    xercesc::DOMDocument* document;

    try {
        domParser->parse("Client.xml");
        document = domParser->getDocument();
    } catch (const xercesc::XMLException& e) {
        char* message = xercesc::XMLString::transcode(e.getMessage());
        YORK_CORE_CRITICAL(message);
        xercesc::XMLString::release(&message);
        return EXIT_FAILURE;
    }
    catch (const xercesc::DOMException& e) {
        char* message = xercesc::XMLString::transcode(e.getMessage());
        YORK_CORE_CRITICAL(message);
        xercesc::XMLString::release(&message);
        return EXIT_FAILURE;
    }

    xercesc::DOMElement* rootNode = document->getDocumentElement();

    xercesc::DOMNodeList* nameNodes = rootNode->getElementsByTagName(xercesc::XMLString::transcode("Name"));

    xercesc::DOMNode* nameNode = nameNodes->item(0);
    xercesc::DOMNode* nameNodeText = nameNode->getFirstChild();

    YORK_CORE_INFO("Found Client: {}", xercesc::XMLString::transcode(nameNodeText->getNodeValue()));

    xercesc::DOMNodeList* layersNodes = rootNode->getElementsByTagName(xercesc::XMLString::transcode("Layers"));

    xercesc::DOMNode* layersNode = layersNodes->item(0);
    xercesc::DOMNodeList* layerNodes = layersNode->getChildNodes();

    xercesc::DOMNode* layerNode = nullptr;

    for (unsigned i = 0; (i < layerNodes->getLength()) && layerNode == nullptr; i++) {
        if (xercesc::XMLString::equals(layerNodes->item(i)->getNodeName(), xercesc::XMLString::transcode("Layer"))) {
            layerNode = layerNodes->item(i);
        }
    }

    xercesc::DOMNodeList* layerNameNodes = layerNode->getChildNodes();
    xercesc::DOMNode* layerNameNode = nullptr;

    for (unsigned i = 0; (i < layerNameNodes->getLength()) && layerNameNode == nullptr; i++) {
        if (xercesc::XMLString::equals(layerNameNodes->item(i)->getNodeName(), xercesc::XMLString::transcode("Name"))) {
            layerNameNode = layerNameNodes->item(i);
        }
    }

    assert(layerNode != nullptr);

    xercesc::DOMNode* layerNameText = layerNameNode->getFirstChild();

    YORK_CORE_DEBUG(xercesc::XMLString::transcode(layerNameText->getNodeValue()));

    york::LayerStack layerStack;
    std::vector<std::reference_wrapper<york::EventHandler>> eventHandlers;

    LayerLoader newlayer { xercesc::XMLString::transcode(layerNameText->getNodeValue()) };
    layerStack.pushLayer(*newlayer);
    eventHandlers.emplace_back(*newlayer);

    curlpp::initialize();

    YORK_CORE_INFO("Initialization took {} seconds on {}!", timer.getTime(), YORK_PLATFORM);

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

    curlpp::terminate();

    delete domParser;
    xercesc::XMLPlatformUtils::Terminate();

    SDL_Quit();

    YORK_CORE_INFO("Shutdown successfully!");
    return EXIT_SUCCESS;
}