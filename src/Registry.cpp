//
// Created by Matthew McCall on 2/20/22.
//

#include "york/Registry.hpp"

namespace {
    york::Registry* s_registry;
}

namespace york {

Registry& Registry::getInstance()
{
    static Registry s_instance;
    return s_registry != nullptr ? *s_registry : s_instance;
}

void registerRegistry(Registry& registry)
{
    s_registry = &registry;
}

}