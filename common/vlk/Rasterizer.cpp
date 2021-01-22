// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes


// Local includes
#include "Rasterizer.h"

using namespace std;

RasterizerBuilder::RasterizerBuilder()
{
    Reset();
}

RasterizerBuilder & RasterizerBuilder::EnableDepthClamp(bool value)
{
    depthClamp = value;
    polygonMode = VK_POLYGON_MODE_FILL;
    return *this;
}

RasterizerBuilder & RolygonMode(VkPolygonMode mode)
{
    polygonMode = mode;
    return *this;
}

RasterizerBuilder & LineWidth(float width)
{
    lineWidth = width;
    return *this;
}

RasterizerBuilder& RasterizerBuilder::Reset()
{
    depthClamp = false;
    lineWidth = 1.0f;
}

VkPipelineRasterizationStateCreateInfo RasterizerBuilder::Build()
{
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = depthClamp ? VK_TRUE : VK_FALSE;
    rasterizer.polygonMode = polygonMode;
    rasterizer.lineWidth = lineWidth;

    return rasterizer;
}
