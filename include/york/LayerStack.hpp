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
    static void pushLayer(Layer& layer);
    static void popLayer(Layer& layer);

    static LayerStack& getStack();

    static std::vector<std::reference_wrapper<Layer>>::iterator begin();
    static std::vector<std::reference_wrapper<Layer>>::iterator end();

};
}

#endif // YORK_LAYERSTACK_HPP
