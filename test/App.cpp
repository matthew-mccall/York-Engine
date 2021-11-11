#include "york/Graphics/Window.hpp"
#include "york/York.hpp"

class App : public york::Application {
    std::shared_ptr<char[]> m_license;
    york::graphics::Window m_window;

public:
    App()
        : m_window("Test")
    {
        york::Asset asset = { "LICENSE", york::Asset::Type::UTF8 };

        auto future = york::async::dispatch(york::Asset::load, asset);
        future.wait();
        m_license = *future.get();

        york::log::info(m_license.get());
    }

    void onEvent(york::Event e) override
    {
        if (e.getType() == york::Event::Type::WindowClose) {
            if (e.getWindowID() == m_window.getID()) {
                m_exit = true;
            }
        }

        Application::onEvent(e);
    }

    void onUpdate(float deltaTime) override
    {
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
