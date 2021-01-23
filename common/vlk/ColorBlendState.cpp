// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
#include <cstdint>
#include <algorithm>

// Local includes
#include "ColorBlendState.h"

using namespace std;

/////////////////////////////////////////////////
/////// ColorBlendStateBuilder
/////////////////////////////////////////////////

ColorBlendStateBuilder::ColorBlendStateBuilder()
{
    Reset();
}

ColorBlendStateBuilder& ColorBlendStateBuilder::LogicOp(bool enable)
{
    logicOpEnable = enable;
    return *this;
}

ColorBlendStateBuilder& ColorBlendStateBuilder::SelectLogicOp(VkLogicOp logicOp)
{
    this->logicOp = logicOp;
    return *this;
}

ColorBlendStateBuilder& ColorBlendStateBuilder::AddAttachment(const VkPipelineColorBlendAttachmentState attachment)
{
    attachments.push_back(attachment);
    return *this;
}

ColorBlendStateBuilder& ColorBlendStateBuilder::BlendConstants(float cR, float cG, float cB, float cA)
{
    blendConstants = {cR, cG, cB, cA};
    return *this;
}

ColorBlendStateBuilder& ColorBlendStateBuilder::Reset()
{
    logicOpEnable = false;
    logicOp = VK_LOGIC_OP_COPY;
    attachments.clear();
    blendConstants.fill(0.0f);
    return *this;
}

VkPipelineColorBlendStateCreateInfo ColorBlendStateBuilder::Build() const
{
    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = logicOpEnable ? VK_TRUE : VK_FALSE;
    colorBlending.logicOp = logicOp;
    colorBlending.attachmentCount = static_cast<uint32_t>(attachments.size());
    colorBlending.pAttachments = attachments.empty() ? nullptr : attachments.data();
    std::copy(blendConstants.begin(), blendConstants.end(), std::begin(colorBlending.blendConstants));

    return colorBlending;
}

/////////////////////////////////////////////////
/////// ColorBlendStateBuilder
/////////////////////////////////////////////////

BlendAttachmentStateBuilder::BlendAttachmentStateBuilder()
{
    Reset();
}

BlendAttachmentStateBuilder& BlendAttachmentStateBuilder::Blend(bool enable)
{
    blendEnable = enable;
    return *this;
}

BlendAttachmentStateBuilder& BlendAttachmentStateBuilder::SrcColorBlendFactor(VkBlendFactor factor)
{
    srcColorBlendFactor = factor;
    return *this;
}

BlendAttachmentStateBuilder& BlendAttachmentStateBuilder::DstColorBlendFactor(VkBlendFactor factor)
{
    dstColorBlendFactor = factor;
    return *this;
}

BlendAttachmentStateBuilder& BlendAttachmentStateBuilder::ColorBlendOp(VkBlendOp blendOp)
{
    colorBlendOp = blendOp;
    return *this;    
}

BlendAttachmentStateBuilder& BlendAttachmentStateBuilder::SrcAlphaBlendFactor(VkBlendFactor factor)
{
    srcAlphaBlendFactor = factor;
    return *this;
}

BlendAttachmentStateBuilder& BlendAttachmentStateBuilder::DstAlphaBlendFactor(VkBlendFactor factor)
{
    dstAlphaBlendFactor = factor;
    return *this;
}

BlendAttachmentStateBuilder& BlendAttachmentStateBuilder::AlphaBlendOp(VkBlendOp blendOp)
{
    alphaBlendOp = blendOp;
    return *this;
}

BlendAttachmentStateBuilder& BlendAttachmentStateBuilder::Reset()
{
    blendEnable = false;
    srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendOp = VK_BLEND_OP_ADD;
    srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    alphaBlendOp = VK_BLEND_OP_ADD;
    return *this;    
}

VkPipelineColorBlendAttachmentState BlendAttachmentStateBuilder::Build() const
{
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.blendEnable = blendEnable;
    colorBlendAttachment.srcColorBlendFactor = srcColorBlendFactor;
    colorBlendAttachment.dstColorBlendFactor = dstColorBlendFactor;
    colorBlendAttachment.colorBlendOp = colorBlendOp;
    colorBlendAttachment.dstAlphaBlendFactor = dstAlphaBlendFactor;
    colorBlendAttachment.dstAlphaBlendFactor = dstAlphaBlendFactor;
    colorBlendAttachment.alphaBlendOp = alphaBlendOp;

    return colorBlendAttachment;
}

