//
// Created by Matthew McCall on 12/19/21.
//

#include "york/Layer.hpp"

namespace york {

void Layer::onEvent(Event e)
{
    switch (e.getType()) {
    case Event::Type::AppTick:
        this->onUpdate(e.getTickTime());
        break;

    case Event::Type::AppRender:
        this->onRender();

    default:
        break;
    }
}

}
