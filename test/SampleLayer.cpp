//
// Created by Matthew McCall on 12/22/21.
//

#include "york/York.hpp"

#include "SampleLayer.hpp"

SampleLayer::SampleLayer()
    : m_window("Test")
    , m_renderer(m_window)
{
}

void SampleLayer::onAttach()
{
    york::Asset license { "LICENSE", york::Asset::Type::UTF8 };

    if (license->empty()) {
        YORK_ERROR("Failed to load license!");
    } else {
        YORK_INFO(license->data());
    }

    york::Asset website { "https://example.com", york::Asset::Type::UTF8, york::Asset::Source::NETWORK };
    // york::log::info(website->data());

    m_renderer.create();

}

void SampleLayer::onEvent(york::Event& e)
{
    if (e.getType() == york::Event::Type::WindowClose) {
        if (e.getWindowID() == m_window.getWindowID()) {
            york::requestExit();
        }
    }
}

void SampleLayer::onUpdate(float deltaTime)
{
}

void SampleLayer::onDetach()
{
    m_window.destroy();
}

void SampleLayer::onRender()
{
    m_renderer.draw();
}
