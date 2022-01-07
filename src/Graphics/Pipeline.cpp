//
// Created by Matthew McCall on 1/3/22.
//

#include "york/Graphics/Pipeline.hpp"

#include <array>
#include <utility>

namespace york::graphics {

Pipeline::Pipeline(SwapChain& swapChain, std::vector<Shader> shaders)
    : m_shaders(std::move(shaders))
    , m_swapChain(swapChain)
    , m_device(m_swapChain.getDevice())
    , m_pipelineLayout(m_device)
    , m_renderPass(m_swapChain)
{
    addDependency(m_swapChain);
    addDependency(m_renderPass);
    addDependency(m_pipelineLayout);
}

void Pipeline::setShaders(std::vector<Shader> shaders)
{
    m_shaders = std::move(shaders);
}

bool Pipeline::createImpl()
{
    std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;
    shaderStages.reserve(m_shaders.size());

    vk::ShaderStageFlagBits stage;
    
    for (Shader& shader : m_shaders) {

        shader.create();

        switch (shader.getType()) {
        case Shader::Type::Vertex:
            stage = vk::ShaderStageFlagBits::eVertex;
            break;
        case Shader::Type::Fragment:
            stage = vk::ShaderStageFlagBits::eFragment;
            break;
        case Shader::Type::TessellationControl:
            stage = vk::ShaderStageFlagBits::eTessellationControl;
            break;
        case Shader::Type::TessellationEvaluation:
            stage = vk::ShaderStageFlagBits::eTessellationEvaluation;
            break;
        case Shader::Type::Geometry:
            stage = vk::ShaderStageFlagBits::eGeometry;
            break;
        case Shader::Type::Compute:
            stage = vk::ShaderStageFlagBits::eCompute;
            break;
        case Shader::Type::RTRayGen:
            stage = vk::ShaderStageFlagBits::eRaygenKHR;
            break;
        case Shader::Type::RTAnyHit:
            stage = vk::ShaderStageFlagBits::eAnyHitKHR;
            break;
        case Shader::Type::RTClosestHit:
            stage = vk::ShaderStageFlagBits::eClosestHitKHR;
            break;
        case Shader::Type::RTMiss:
            stage = vk::ShaderStageFlagBits::eMissKHR;
            break;
        case Shader::Type::RTIntersection:
            stage = vk::ShaderStageFlagBits::eIntersectionKHR;
            break;
        case Shader::Type::RTCallable:
            stage = vk::ShaderStageFlagBits::eCallableKHR;

            break;
        }

        shaderStages.push_back({{}, stage, *shader, "main"});

    }

    vk::PipelineVertexInputStateCreateInfo vertexInputStateCreateInfo { {}, {} };
    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo {{}, vk::PrimitiveTopology::eTriangleList, VK_FALSE};

    vk::Extent2D swapExtent = m_swapChain.getExtent();

    vk::Viewport viewport {
        0,
        0,
        static_cast<float>(swapExtent.width),
        static_cast<float>(swapExtent.height),
        0,
        1
    };

    std::array<vk::Viewport, 1> viewports { viewport };

    vk::Rect2D scissor { {0, 0}, swapExtent};

    std::array<vk::Rect2D, 1> scissors { scissor };

    vk::PipelineViewportStateCreateInfo viewportStateCreateInfo {{}, viewports, scissors};

    vk::PipelineRasterizationStateCreateInfo rasterizationStateCreateInfo {
        {},
        VK_FALSE,
        VK_FALSE,
        vk::PolygonMode::eFill,
        vk::CullModeFlagBits::eBack,
        vk::FrontFace::eClockwise,
        VK_FALSE,
        0,
        0,
        0,
        1
    };

    vk::PipelineMultisampleStateCreateInfo multisampleStateCreateInfo {{}, vk::SampleCountFlagBits::e1, VK_FALSE, 1};

    vk::PipelineColorBlendAttachmentState colorBlendAttachmentState {
        VK_TRUE,
        vk::BlendFactor::eSrcAlpha,
        vk::BlendFactor::eOneMinusSrcAlpha,
        vk::BlendOp::eAdd,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
    };

    std::array<vk::PipelineColorBlendAttachmentState, 1> colorBlendAttachmentStates { colorBlendAttachmentState };

    vk::PipelineColorBlendStateCreateInfo colorBlendStateCreateInfo {
        {},
        VK_FALSE,
        vk::LogicOp::eCopy,
        colorBlendAttachmentStates,
        {0, 0, 0, 0}
    };

    /*
    std::array<vk::DynamicState, 2> dynamicStates = { vk::DynamicState::eViewport, vk::DynamicState::eLineWidth };
    vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo { {}, dynamicStates };
    */

    vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo {
        {},
        shaderStages,
        &vertexInputStateCreateInfo,
        &inputAssemblyStateCreateInfo,
        nullptr,
        &viewportStateCreateInfo,
        &rasterizationStateCreateInfo,
        &multisampleStateCreateInfo,
        nullptr,
        &colorBlendStateCreateInfo,
        nullptr,
        *m_pipelineLayout,
        *m_renderPass,
        0
    };

    m_handle = m_device->createGraphicsPipeline(VK_NULL_HANDLE, graphicsPipelineCreateInfo).value;

    for (Shader& shader : m_shaders) {
        shader.destroy();
    }

    return true;
}

void Pipeline::destroyImpl()
{
    m_device->destroy(m_handle);
}

}
