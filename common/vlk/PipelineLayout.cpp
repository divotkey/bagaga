// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
#include <stdexcept>
#include <cassert>
#include <string>

// Local includes
#include "LogicalDevice.h"
#include "PipelineLayout.h"

using namespace std;

/////////////////////////////////////////////////
/////// PipelineLayout
/////////////////////////////////////////////////

PipelineLayout::PipelineLayout(VkPipelineLayout handle, shared_ptr<LogicalDevice> device)
    : pipelineLayout(handle)
    , device(device)
{
    assert(pipelineLayout);
    assert(device);
}

PipelineLayout::~PipelineLayout()
{
    vkDestroyPipelineLayout(*device, pipelineLayout, nullptr);
}


/////////////////////////////////////////////////
/////// PipelineLayoutBuilder
/////////////////////////////////////////////////

PipelineLayoutBuilder::PipelineLayoutBuilder()
{
    Reset();
}

PipelineLayoutBuilder& PipelineLayoutBuilder::Reset()
{
    return *this;
}

unique_ptr<PipelineLayout> PipelineLayoutBuilder::Build(shared_ptr<LogicalDevice> device) const
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    //TODO make configurable.
    pipelineLayoutInfo.setLayoutCount = 0; 
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    VkPipelineLayout handle;
    VkResult res = vkCreatePipelineLayout(*device, &pipelineLayoutInfo, nullptr, &handle);
    if (res != VK_SUCCESS) {
        throw std::runtime_error("Failed to create pipeline layout, error " + to_string(res));
    }

    return unique_ptr<PipelineLayout>(new PipelineLayout(handle, device));
}
