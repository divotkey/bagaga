// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
#include <cassert>

// Local includes
#include "LogicalDevice.h"
#include "GraphicsPipeline.h"

using namespace std;

/////////////////////////////////////////////////
/////// GraphicsPipeline
/////////////////////////////////////////////////

GraphicsPipeline::GraphicsPipeline(VkPipeline handle, std::shared_ptr<LogicalDevice> device)
    : pipeline(handle), device(device)
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
}

GraphicsPipelineBuilder & GraphicsPipelineBuilder::VertexInputInfo(const VkPipelineVertexInputStateCreateInfo & info)
{
    vertexInputInfo = info;
    return *this;
}

GraphicsPipelineBuilder & GraphicsPipelineBuilder::InputAssembly(const VkPipelineInputAssemblyStateCreateInfo & assembly)
{
    inputAssembly = assembly;
    return *this;
}

std::unique_ptr<GraphicsPipeline> GraphicsPipelineBuilder::Build(std::shared_ptr<LogicalDevice> device) const
{
    ValidateConfiguration();

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    
    pipelineInfo.pVertexInputState = &vertexInputInfo.value();
    pipelineInfo.pInputAssemblyState = &inputAssembly.value();

    return nullptr;
}

void GraphicsPipelineBuilder::ValidateConfiguration() const
{
    if (!vertexInputInfo.has_value()) {
        throw std::logic_error("Unable to build graphics pipeline, vertex input info not specified");
    }

    if (!inputAssembly.has_value()) {
        throw std::logic_error("Unable to build graphics pipeline, input asselly not specified");
    }
}

