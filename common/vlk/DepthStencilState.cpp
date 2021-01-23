// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes

// Local includes
#include "DepthStencilState.h"

using namespace std;

DepthStencilStateBuilder::DepthStencilStateBuilder()
{
    Reset();
}

DepthStencilStateBuilder& DepthStencilStateBuilder::DepthTest(bool value)
{
    depthTest = value;
    return *this;
}

DepthStencilStateBuilder& DepthStencilStateBuilder::DepthWrite(bool value)
{
    depthWrite = value;
    return *this;
}

DepthStencilStateBuilder& DepthStencilStateBuilder::DepthCompareOp(VkCompareOp compareOp)
{
    depthCompareOp = compareOp;
    return *this;
}

DepthStencilStateBuilder& DepthStencilStateBuilder::DepthBoundsTest(bool value)
{
    depthBoundsTest = value;
    return *this;
}

DepthStencilStateBuilder& DepthStencilStateBuilder::StencilTest(bool value)
{
    stencilTest = value;
    return *this;
}

DepthStencilStateBuilder& DepthStencilStateBuilder::Front(VkStencilOpState opState)
{
    front = opState;
    return *this;
}

DepthStencilStateBuilder& DepthStencilStateBuilder::Back(VkStencilOpState opState)
{
    back = opState;
    return *this;
}

DepthStencilStateBuilder& DepthStencilStateBuilder::MinDepthBounds(float bounds)
{
    minDepthBounds = bounds;
    return *this;
}

DepthStencilStateBuilder& DepthStencilStateBuilder::MaxDepthBounds(float bounds)
{
    maxDepthBounds = bounds;
    return *this;
}

DepthStencilStateBuilder& DepthStencilStateBuilder::Reset()
{
    // TODO use reasonable default values.
    depthTest = false;
    depthWrite = false;
    depthCompareOp = VK_COMPARE_OP_NEVER;
    depthBoundsTest = false;
    stencilTest = false;
    front = VkStencilOpState{};
    back = VkStencilOpState{};
    minDepthBounds = 0.0f;
    maxDepthBounds = 1.0f;
    return *this;
}

VkPipelineDepthStencilStateCreateInfo DepthStencilStateBuilder::Build() const
{
    VkPipelineDepthStencilStateCreateInfo detphStencilState{};
    detphStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    detphStencilState.depthTestEnable = depthTest ? VK_TRUE : VK_FALSE;
    detphStencilState.depthWriteEnable = depthWrite ? VK_TRUE : VK_FALSE;
    detphStencilState.depthCompareOp = depthCompareOp;
    detphStencilState.depthBoundsTestEnable = depthBoundsTest ? VK_TRUE : VK_FALSE;
    detphStencilState.stencilTestEnable = stencilTest ? VK_TRUE : VK_FALSE;
    detphStencilState.front = front;
    detphStencilState.back = back;
    detphStencilState.minDepthBounds = minDepthBounds;
    detphStencilState.maxDepthBounds = maxDepthBounds;

    return detphStencilState;
}
