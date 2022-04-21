//
// Created by Matthew McCall on 2/20/22.
//

#ifndef YORK_REGISTRY_HPP
#define YORK_REGISTRY_HPP

#include <vector>
#include <functional>

namespace york {

class EventHandler;
struct Event;

/**
 * A singleton class for sharing data between multiple instances of York-Engine.
 *
 * Currently, the Runtime links with York-Engine. On most modern operating systems, this results in the Engine library being loaded twice, with the Runtime and your Client have their own copies loaded. As a result these two copies of the Engine have their own memory allocated. Thus, internal data structures created in one copy will not be available in the other copy. As such, the `Registry` will contain the necessary data initialized by the Runtime for use in your client. You need to accept this `Registry` and pass it to the base constructor of the `Layer`.
 */
class Registry {
    friend EventHandler;
    friend void broadcastEvent(Event e);

public:
    Registry(const Registry&) = delete;

    /**
     * Please do not call this method in your client.
     *
     * The Registry will be passed in the createFunction.
     *
     * @return The Registry
     */
    static Registry& getInstance();

private:
    Registry() = default;
    std::vector<std::reference_wrapper<EventHandler>> m_eventHandlers;
};

extern "C" void registerRegistry(Registry& registry);

}

#endif // YORK_REGISTRY_HPP
