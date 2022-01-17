//
// Created by Matthew McCall on 12/22/21.
//

#ifndef YORKTEST_SAMPLELAYER_HPP
#define YORKTEST_SAMPLELAYER_HPP

#include "york/Graphics/Device.hpp"
#include "york/Graphics/Instance.hpp"
#include "york/Graphics/Pipeline.hpp"
#include "york/Graphics/Shader.hpp"
#include "york/Graphics/Surface.hpp"
#include "york/Graphics/SwapChain.hpp"
#include "york/Window.hpp"
#include "york/Layer.hpp"
#include "york/Renderer/Renderer.hpp"

class SampleLayer : public york::Layer {
public:
    SampleLayer();

    void onAttach() override;
    void onEvent(york::Event& e) override;
    void onUpdate(float deltaTime) override;
    void onDetach() override;

private:
    york::graphics::Window m_window;
    york::Renderer m_renderer;
};

#endif // YORKTEST_SAMPLELAYER_HPP
