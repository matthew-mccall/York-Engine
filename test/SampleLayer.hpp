//
// Created by Matthew McCall on 12/22/21.
//

#ifndef YORKTEST_SAMPLELAYER_HPP
#define YORKTEST_SAMPLELAYER_HPP

#include "york/Graphics/Device.hpp"
#include "york/Graphics/Instance.hpp"
#include "york/Graphics/Shader.hpp"
#include "york/Graphics/Surface.hpp"
#include "york/Graphics/SwapChain.hpp"
#include "york/Graphics/Window.hpp"
#include "york/Layer.hpp"

class SampleLayer : public york::Layer {
public:
    SampleLayer();

    void onAttach() override;
    void onEvent(york::Event& e) override;
    void onUpdate(float deltaTime) override;
    void onDetach() override;

private:
    york::graphics::Window m_window;
    york::graphics::Instance m_instance;
    york::graphics::Surface m_surface;
    york::graphics::Device m_device;
    york::graphics::SwapChain m_swapchain;
};

#endif // YORKTEST_SAMPLELAYER_HPP
