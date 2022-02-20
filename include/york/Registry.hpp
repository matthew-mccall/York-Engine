//
// Created by Matthew McCall on 2/20/22.
//

#ifndef YORK_REGISTRY_HPP
#define YORK_REGISTRY_HPP

#include <vector>
#include <functional>

namespace york {

class EventHandler;

class Registry {
    friend EventHandler;
    friend void dispatchEvents();

public:
    Registry(const Registry&) = delete;
    static Registry& getInstance();

private:
    Registry() = default;
    std::vector<std::reference_wrapper<EventHandler>> m_eventHandlers;
};

}

#endif // YORK_REGISTRY_HPP
