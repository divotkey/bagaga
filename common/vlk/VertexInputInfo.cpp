// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
#include <cassert>

// Local includes
#include "VertexInputInfo.h"

using namespace std;

VertexInputInfoBuilder::VertexInputInfoBuilder()
{
    Reset();
}

VertexInputInfoBuilder & VertexInputInfoBuilder::Reset()
{
    return *this;
}

VkPipelineVertexInputStateCreateInfo VertexInputInfoBuilder::Build() const
{
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    // TODO make configurable.
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

    return vertexInputInfo;
}
