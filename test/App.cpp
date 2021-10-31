#include <york/York.hpp>

class App : public york::Application {
public:
    App()
    {
        york::asset::Asset asset = { "LICENSE", york::asset::Type::UTF8 };

        auto future = york::async::dispatch(york::asset::load, asset);
        auto license = future.get().value();

        york::log::trace(license.get());
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
