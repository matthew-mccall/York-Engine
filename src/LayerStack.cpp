//
// Created by Matthew McCall on 12/19/21.
//

#include "york/LayerStack.hpp"

namespace york {

void LayerStack::pushLayer(Layer& layer)
{
    layer.onAttach();
    m_layers.emplace_back(layer);
}

void LayerStack::popLayer(Layer& layer)
{
    for (auto i = m_layers.begin(); i != m_layers.end(); i++) {
        if (layer == *i) {
            i->get().onDetach();
            m_layers.erase(i);
            break;
        }
    }
}

std::vector<std::reference_wrapper<Layer>>::iterator LayerStack::begin()
{
    return m_layers.begin();
}

std::vector<std::reference_wrapper<Layer>>::iterator LayerStack::end()
{
    return m_layers.end();
}

LayerStack::~LayerStack()
{
    for (Layer& layer : m_layers) {
        layer.onDetach();
    }
}

}