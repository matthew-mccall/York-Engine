#include <york/Application.hpp>

using namespace york;

void Application::onEvent(Event e)
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

bool Application::getExit() const
{
    return m_exit;
}