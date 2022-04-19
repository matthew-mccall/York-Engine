//
// Created by Matthew McCall on 12/22/21.
//

#include "york/Renderer/Renderer.hpp"
#include "york/Window.hpp"
#include "york/York.hpp"

class SampleLayer : public york::Layer {
public:
    explicit SampleLayer()
        : m_window("Test")
        , m_renderer(m_window)
    {
    }

    void onAttach() override
    {
        york::Asset license { "LICENSE", york::Asset::Type::UTF8 };

        if (license->empty()) {
            YORK_ERROR("Failed to load license!");
        } else {
            YORK_INFO(license->data());
        }

        // york::Asset website { "https://example.com", york::Asset::Type::UTF8, york::Asset::Source::NETWORK };
        // york::log::info(website->data());
    }

    void onEvent(york::Event& e) override
    {
        if (e.getType() == york::Event::Type::WindowClose) {
            if (e.getWindowID() == m_window.getWindowID()) {
                requestExit();
            }
        }
    }

    void onUpdate(float deltaTime) override {}

    void onRender() override
    {
        if (!m_renderer.draw()) {
            requestExit();
        }
    }

    void onDetach() override
    {
        m_window.close();
    }

private:
    york::graphics::Window m_window;
    york::Renderer m_renderer;
};

extern "C" york::Layer* createLayer()
{
    return new SampleLayer();
}