// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
#include <cassert>

// Local includes
#include "VertexInputInfo.h"

using namespace std;


/////////////////////////////////////////////////
/////// VertexInputState
/////////////////////////////////////////////////

VertexInputState::VertexInputState (
    const VkPipelineVertexInputStateCreateInfo & vertexInputInfo,
    const vector<VkVertexInputBindingDescription> & bindingDescriptions, 
    const vector<VkVertexInputAttributeDescription> & attributeDescriptions)
    : vertexInputInfo(vertexInputInfo)
    , bindingDescriptions(bindingDescriptions)
    , attributeDescriptions(attributeDescriptions)
{
    this->vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
    this->vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.empty() ? nullptr : bindingDescriptions.data();
    this->vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    this->vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.empty() ? nullptr : attributeDescriptions.data();
}

/////////////////////////////////////////////////
/////// VertexInputInfoBuilder
/////////////////////////////////////////////////

VertexInputInfoBuilder::VertexInputInfoBuilder()
{
    Reset();
}

VertexInputInfoBuilder& VertexInputInfoBuilder::AddVertexBindingDescription(VkVertexInputBindingDescription bindingDesc)
{
    bindingDescriptions.push_back(bindingDesc);
    return *this;
}

VertexInputInfoBuilder& VertexInputInfoBuilder::AddVertexAtttributeDescription(VkVertexInputAttributeDescription attributeDesc)
{
    attributeDescriptions.push_back(attributeDesc);
    return *this;
}

VertexInputInfoBuilder & VertexInputInfoBuilder::Reset()
{
    bindingDescriptions.clear();
    attributeDescriptions.clear();
    flags = 0;
    return *this;
}

VertexInputState VertexInputInfoBuilder::Build() const
{
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.flags = flags;

    return VertexInputState(vertexInputInfo, bindingDescriptions, attributeDescriptions);
}
