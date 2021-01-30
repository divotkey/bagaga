// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
#include <cassert>

// Local includes
#include "LogicalDevice.h"
#include "PipelineLayout.h"
#include "GraphicsPipeline.h"

using namespace std;

/////////////////////////////////////////////////
/////// GraphicsPipeline
/////////////////////////////////////////////////

GraphicsPipeline::GraphicsPipeline(VkPipeline handle, shared_ptr<LogicalDevice> device, shared_ptr<PipelineLayout> layout)
    : pipeline(handle), device(device), layout(layout)
{
    assert(pipeline);
    assert(device);
}

GraphicsPipeline::~GraphicsPipeline()
{
    if (pipeline) {
        vkDestroyPipeline(*device, pipeline, nullptr);
    }
}

/////////////////////////////////////////////////
/////// GraphicsPipelineBuilder
/////////////////////////////////////////////////

GraphicsPipelineBuilder::GraphicsPipelineBuilder()
{
    Reset();
}

GraphicsPipelineBuilder & GraphicsPipelineBuilder::Reset()
{   
    vertexInputInfo.reset();
    inputAssembly.reset();
    viewportState.reset();
    rasterizer.reset();
    multisampling.reset();
    depthStencil.reset();
    colorBlending.reset();
    dynamicState.reset();
    pipelineLayout = nullptr;
    renderPass = VK_NULL_HANDLE;
    subpass = 0;
    basePipeline = VK_NULL_HANDLE;
    basePipelineIndex = -1;
    shaderStages.clear();
    return *this;
}

GraphicsPipelineBuilder & GraphicsPipelineBuilder::VertexInputInfo(const VkPipelineVertexInputStateCreateInfo & info)
{
    vertexInputInfo = info;
    return *this;
}

GraphicsPipelineBuilder & GraphicsPipelineBuilder::InputAssembly(const VkPipelineInputAssemblyStateCreateInfo & info)
{
    inputAssembly = info;
    return *this;
}

GraphicsPipelineBuilder & GraphicsPipelineBuilder::ViewportState(const ViewportStateInfo & info)
{
    viewportState = info;
    return *this;
}

GraphicsPipelineBuilder & GraphicsPipelineBuilder::Rasterizer(const VkPipelineRasterizationStateCreateInfo & info)
{
    rasterizer = info;
    return *this;
}

GraphicsPipelineBuilder & GraphicsPipelineBuilder::Multisampling(const VkPipelineMultisampleStateCreateInfo & info)
{
    multisampling = info;
    return *this;
}

GraphicsPipelineBuilder & GraphicsPipelineBuilder::DepthStencil(const VkPipelineDepthStencilStateCreateInfo & info)
{
    depthStencil = info;
    return *this;
}

GraphicsPipelineBuilder & GraphicsPipelineBuilder::ColorBlending(const ColorBlendStateInfo & info)
{
    colorBlending = info;
    return *this;
}

GraphicsPipelineBuilder & GraphicsPipelineBuilder::DynamicState(const VkPipelineDynamicStateCreateInfo & info)
{
    dynamicState = info;
    return *this;
}

GraphicsPipelineBuilder & GraphicsPipelineBuilder::Layout(shared_ptr<PipelineLayout> layout)
{
    pipelineLayout = layout;
    return *this;
}

GraphicsPipelineBuilder & GraphicsPipelineBuilder::RenderPass(VkRenderPass renderPass)
{
    this->renderPass = renderPass;
    return *this;
}

GraphicsPipelineBuilder & GraphicsPipelineBuilder::Subpass(uint32_t subpass)
{
    this->subpass = subpass;
    return *this;
}

GraphicsPipelineBuilder & GraphicsPipelineBuilder::BasePipeline(VkPipeline basePipeline)
{
    this->basePipeline = basePipeline;
    return *this;
}

GraphicsPipelineBuilder & GraphicsPipelineBuilder::BasePipelineIndex(int32_t index)
{
    basePipelineIndex = index;
    return *this;
}

GraphicsPipelineBuilder & GraphicsPipelineBuilder::AddShaderStage(const ShaderStageInfo & stage)
{
    shaderStages.push_back(stage);
    return *this;
}

unique_ptr<GraphicsPipeline> GraphicsPipelineBuilder::Build(shared_ptr<LogicalDevice> device) const
{
    ValidateConfiguration();

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());

    vector<VkPipelineShaderStageCreateInfo> stages;
    for (const auto & stage : shaderStages) {
        stages.push_back(stage.GetInfo());
    }
    pipelineInfo.pStages = stages.data();

    pipelineInfo.pVertexInputState = &vertexInputInfo.value();
    pipelineInfo.pInputAssemblyState = &inputAssembly.value();
    pipelineInfo.pViewportState = viewportState.value();
    pipelineInfo.pRasterizationState = &rasterizer.value();
    pipelineInfo.pMultisampleState = &multisampling.value();
    pipelineInfo.pDepthStencilState = depthStencil.has_value() ? &depthStencil.value() : nullptr;
    pipelineInfo.pColorBlendState = colorBlending.value();
    pipelineInfo.pDynamicState = dynamicState.has_value() ? &dynamicState.value() : nullptr;

    pipelineInfo.layout = *pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = subpass;

    pipelineInfo.basePipelineHandle = basePipeline;
    pipelineInfo.basePipelineIndex = basePipelineIndex;


    VkPipeline handle;
    VkResult res = vkCreateGraphicsPipelines(
        *device, 
        VK_NULL_HANDLE, 
        1, 
        &pipelineInfo, 
        nullptr, 
        &handle);
    
    if (res != VK_SUCCESS) {
        throw runtime_error("failed to create graphics pipeline!");
    }

    return unique_ptr<GraphicsPipeline>(new GraphicsPipeline(handle, device, pipelineLayout));
}

void GraphicsPipelineBuilder::ValidateConfiguration() const
{
    if (!vertexInputInfo.has_value()) {
        throw logic_error("Unable to build graphics pipeline, vertex input info not specified");
    }

    if (!inputAssembly.has_value()) {
        throw logic_error("Unable to build graphics pipeline, input assembly not specified");
    }

    if (!viewportState.has_value()) {
        // TODO viewport state can be empty, if rastererization is disabled.
        throw logic_error("Unable to build graphics pipeline, viewport state not specified");
    }

    if (!rasterizer.has_value()) {
        throw logic_error("Unable to build graphics pipeline, rasterization state not specified");
    }

    if (!multisampling.has_value()) {
        throw logic_error("Unable to build graphics pipeline, multisampling state not specified");
    }

    if (!colorBlending.has_value()) {
        throw logic_error("Unable to build graphics pipeline, color blend state not specified");
    }

    if (!pipelineLayout) {
        throw logic_error("Unable to build graphics pipeline, pipeline layout not specified");
    }

    if (renderPass == VK_NULL_HANDLE) {
        throw logic_error("Unable to build graphics pipeline, render pass not specified");
    }

    if (shaderStages.empty()) {
        throw logic_error("Unable to build graphics pipeline, no shader stages specified");
    }

}

