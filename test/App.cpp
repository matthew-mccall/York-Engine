#include <york/York.hpp>

class App : public york::Application {
public:
    App()
    {
        york::log::info("hello!");
        york::asset::Asset asset = { "LICENSE", york::asset::Type::UTF8 };
        std::shared_ptr<char[]> license = *(york::asset::load(asset).get());
        york::log::info(license.get());
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
