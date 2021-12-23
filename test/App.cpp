#include "york/Application.hpp"
#include "york/Entry.hpp"

#include "SampleLayer.hpp"

class App : public york::Application {
    SampleLayer layer;

public:
    App()
    {
        pushLayer(layer);
    }
};

york::Application* york::createApplication()
{
    return new App();
}
