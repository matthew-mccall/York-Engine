#include "york/Graphics/Instance.hpp"
#include "york/Graphics/Window.hpp"
#include "york/York.hpp"

class App : public york::Application {
    std::shared_ptr<char[]> m_license;
    york::graphics::Window m_window;
    york::graphics::Instance m_instance;

public:
    App()
        : m_window("Test")
        , m_instance(m_window)
    {
        york::Asset asset = { "LICENSE", york::Asset::Type::UTF8 };

        auto future = york::async::dispatch(york::Asset::load, asset);
        m_license = *future.get();

        york::log::info(m_license.get());

        m_instance.create();
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

    ~App() override
    {
        m_instance.destroy();
    }
};

york::Application* york::createApplication()
{
    return new App();
}
