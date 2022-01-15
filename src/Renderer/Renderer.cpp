//
// Created by Matthew McCall on 1/8/22.
//

#include "york/Asset.hpp"
#include "york/Graphics/ImageView.hpp"
#include "york/Renderer/Renderer.hpp"

namespace york {

Renderer::Renderer(graphics::Window& window)
    : m_window(window)
    , m_instance(m_window)
    , m_surface(m_instance, m_window)
    , m_device(m_instance, m_surface)
    , m_swapchain(m_device, m_window, m_surface)
    , m_renderPass(m_swapchain)
    , m_pipeline(m_swapchain, m_renderPass)
{
    addDependency(m_window);
    // addDependency(m_instance);
    // addDependency(m_surface);
    // addDependency(m_device);

    york::Asset vert { "test/assets/shader.vert", york::Asset::Type::SHADER_VERT_GLSL };
    std::string vertSrc = std::string { vert->data() };
    york::graphics::Shader vertShader { m_device, vertSrc, york::graphics::Shader::Type::Vertex };

    york::Asset frag { "test/assets/shader.frag", york::Asset::Type::SHADER_VERT_GLSL };
    std::string fragSrc = std::string { frag->data() };
    york::graphics::Shader fragShader { m_device, fragSrc, york::graphics::Shader::Type::Fragment };

    std::vector<york::graphics::Shader> shaders { vertShader, fragShader };

    m_pipeline.setShaders(shaders);
}

bool Renderer::createImpl()
{
    for (york::graphics::ImageView& imageView : m_swapchain.getImageViews()) {
        m_framebuffers.emplace_back(m_renderPass, imageView);
    }

    return true;
}

void Renderer::destroyImpl()
{
    for (york::graphics::Framebuffer& framebuffer: m_framebuffers) {
        // framebuffer.destroy();
    }
    m_framebuffers.clear();
}
}