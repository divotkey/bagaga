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

VertexInputInfo::VertexInputInfo (
    const VkPipelineVertexInputStateCreateInfo & vertexInputInfo,
    const vector<VkVertexInputBindingDescription> & bindingDescriptions, 
    const vector<VkVertexInputAttributeDescription> & attributeDescriptions)
    : vertexInputInfo(vertexInputInfo)
    , bindingDescriptions(bindingDescriptions)
    , attributeDescriptions(attributeDescriptions)
{
    UpdateData();
}

VertexInputInfo::VertexInputInfo(const VertexInputInfo & o)
    : vertexInputInfo(o.vertexInputInfo), bindingDescriptions(o.bindingDescriptions), attributeDescriptions(o.attributeDescriptions)
{
    UpdateData();
}

VertexInputInfo & VertexInputInfo::operator= (const VertexInputInfo & rhs)
{
    vertexInputInfo = rhs.vertexInputInfo;
    bindingDescriptions = rhs.bindingDescriptions;
    attributeDescriptions = rhs.attributeDescriptions;
    UpdateData();
    return *this;
}

void VertexInputInfo::UpdateData()
{
    vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.empty() ? nullptr : bindingDescriptions.data();
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.empty() ? nullptr : attributeDescriptions.data();
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

VertexInputInfoBuilder& VertexInputInfoBuilder::AddVertexAttributeDescription(VkVertexInputAttributeDescription attributeDesc)
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

VertexInputInfo VertexInputInfoBuilder::Build() const
{
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.flags = flags;

    return VertexInputInfo(vertexInputInfo, bindingDescriptions, attributeDescriptions);
}
