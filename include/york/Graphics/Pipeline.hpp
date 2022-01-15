//
// Created by Matthew McCall on 1/3/22.
//

#ifndef YORK_GRAPHICS_PIPELINE_HPP
#define YORK_GRAPHICS_PIPELINE_HPP

#include <vulkan/vulkan.hpp>

#include "Handle.hpp"
#include "PipelineLayout.hpp"
#include "RenderPass.hpp"
#include "Shader.hpp"
#include "SwapChain.hpp"
#include "RenderPass.hpp"

namespace york::graphics {

class Pipeline : Handle<vk::Pipeline> {
public:
    explicit Pipeline(SwapChain& swapChain, RenderPass& renderPass, std::vector<Shader> shaders = {});
    void setShaders(std::vector<Shader> shaders);

protected:
    bool createImpl() override;
    void destroyImpl() override;

private:
    std::vector<Shader> m_shaders;
    SwapChain& m_swapChain;
    Device& m_device;
    PipelineLayout m_pipelineLayout;
    RenderPass& m_renderPass;
};

}


#endif // YORK_GRAPHICS_PIPELINE_HPP
