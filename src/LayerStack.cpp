//
// Created by Matthew McCall on 12/19/21.
//

#include "york/LayerStack.hpp"

namespace {
    york::LayerStack* s_stack;
    std::vector<std::reference_wrapper<york::Layer>> s_layers;
}

namespace york {

void LayerStack::pushLayer(Layer& layer)
{
    s_layers.emplace_back(layer);
}

void LayerStack::popLayer(Layer& layer)
{
    for (auto i = s_layers.begin(); i != s_layers.end(); i++) {
        if (layer.getID())
        i->get().onDetach();
        s_layers.erase(i);
    }
}

LayerStack& LayerStack::getStack()
{
    if (s_stack == nullptr) {
        s_stack = new LayerStack();
    }

    return *s_stack;
}

std::vector<std::reference_wrapper<Layer>>::iterator LayerStack::begin()
{
    return s_layers.begin();
}

std::vector<std::reference_wrapper<Layer>>::iterator LayerStack::end()
{
    return s_layers.end();
}

}