#include <york/Application.hpp>

namespace york {

LayerStack& Application::getLayerStack()
{
    return m_defaultLayerStack;
}

void Application::pushLayer(Layer& layer)
{
    m_defaultLayerStack.pushLayer(layer);
}

void Application::popLayer(Layer& layer)
{
    m_defaultLayerStack.popLayer(layer);
}

}