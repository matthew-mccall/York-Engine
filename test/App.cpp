#include <york/York.hpp>

class App : public york::Application {
    std::shared_ptr<char[]> m_license;

public:
    App()
    {
        york::Asset asset = { "LICENSE", york::Asset::Type::UTF8 };

        auto future = york::async::dispatch(york::Asset::load, asset);
        future.wait();
        m_license = *future.get();

        york::log::trace(m_license.get());
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
