//
// Created by Matthew McCall on 12/19/21.
//

#ifndef YORK_LAYERSTACK_HPP
#define YORK_LAYERSTACK_HPP

#include <functional>
#include <vector>

#include "Layer.hpp"

namespace york {

class LayerStack {
public:
    LayerStack() = default;

    void pushLayer(Layer& layer);
    void popLayer(Layer& layer);

    std::vector<std::reference_wrapper<Layer>>::iterator begin();
    std::vector<std::reference_wrapper<Layer>>::iterator end();

    ~LayerStack();

private:
    std::vector<std::reference_wrapper<Layer>> m_layers;

};
}

#endif // YORK_LAYERSTACK_HPP
