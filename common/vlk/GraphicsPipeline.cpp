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

GraphicsPipelineBuilder & GraphicsPipelineBuilder::Flags(VkPipelineCreateFlags flags)
{
    this->flags = flags;
    return *this;
}

GraphicsPipelineBuilder & GraphicsPipelineBuilder::VertexInputState(const VertexInputInfo & info)
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

GraphicsPipelineBuilder & GraphicsPipelineBuilder::AddShaderStage(const ShaderStageInfo & stage)
{
    shaderStages.push_back(stage);
    return *this;
}

GraphicsPipelineBuilder & GraphicsPipelineBuilder::Reset()
{   
    flags = 0;
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
    shaderStages.clear();
    return *this;
}

unique_ptr<GraphicsPipeline> GraphicsPipelineBuilder::Build(shared_ptr<LogicalDevice> device) const
{
    ValidateConfiguration();

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.flags = flags;
    pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());

    vector<VkPipelineShaderStageCreateInfo> stages;
    for (const auto & stage : shaderStages) {
        stages.push_back(stage.GetInfo());
    }
    pipelineInfo.pStages = stages.data();

    pipelineInfo.pVertexInputState = &vertexInputInfo.value().GetInfo();
    pipelineInfo.pInputAssemblyState = &inputAssembly.value();
    pipelineInfo.pViewportState = viewportState.has_value() ? &viewportState.value().GetInfo() : nullptr;
    pipelineInfo.pRasterizationState = rasterizer.has_value() ? &rasterizer.value() : nullptr;
    pipelineInfo.pMultisampleState = multisampling.has_value() ? &multisampling.value() : nullptr;
    pipelineInfo.pDepthStencilState = depthStencil.has_value() ? &depthStencil.value() : nullptr;
    pipelineInfo.pColorBlendState = colorBlending.has_value() ? &colorBlending.value().GetInfo() : nullptr;
    pipelineInfo.pDynamicState = dynamicState.has_value() ? &dynamicState.value() : nullptr;

    pipelineInfo.layout = *pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = subpass;

    pipelineInfo.basePipelineHandle = basePipeline;
    pipelineInfo.basePipelineIndex = -1;


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
    if ((basePipeline != VK_NULL_HANDLE) && !(flags & VK_PIPELINE_CREATE_DERIVATIVE_BIT)) {
        throw logic_error("Unable to build graphics derived pipeline, VK_PIPELINE_CREATE_DERIVATIVE_BIT not set");
    }

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

    if ((basePipeline == VK_NULL_HANDLE) && (renderPass == VK_NULL_HANDLE)) {
        throw logic_error("Unable to build graphics pipeline, render pass not specified");
    }

    if (shaderStages.empty()) {
        throw logic_error("Unable to build graphics pipeline, no shader stages specified");
    }

}

