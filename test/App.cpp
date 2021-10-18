#include <york/York.hpp>

class App : public york::Application {
public:
    App()
    {
        york::log::info("hello!");
    }

    void onUpdate(float deltaTime) override
    {
        m_exit = true;
    }

    void onRender() override
    {
    }

    ~App()
    {
    }
};

york::Application* york::createApplication()
{
    return new App();
}
