#include <york/Application.hpp>

using namespace york;

Application::Application() { }

bool Application::getExit() const
{
    return m_exit;
}

Application::~Application() { }