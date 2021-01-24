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
    return *this;
}

RasterizerBuilder & RasterizerBuilder::RolygonMode(VkPolygonMode mode)
{
    polygonMode = mode;
    return *this;
}

RasterizerBuilder & RasterizerBuilder::LineWidth(float width)
{
    lineWidth = width;
    return *this;
}

RasterizerBuilder & RasterizerBuilder::CullMode(VkCullModeFlags flags)
{
    cullMode = flags;
    return *this;
}

RasterizerBuilder & RasterizerBuilder::FrontFace(VkFrontFace frontFace)
{
    this->frontFace = frontFace;
    return *this;
}

RasterizerBuilder & RasterizerBuilder::DepthBias(bool value)
{
    depthBias = value;
    return *this;
}

RasterizerBuilder & RasterizerBuilder::DepthBiasConstantFactor(float constantFactor)
{
    depthBiasConstantFactor = constantFactor;
    return *this;
}

RasterizerBuilder & RasterizerBuilder::DepthBiasClamp(float biasClamp)
{
    depthBiasClamp = biasClamp;
    return *this;
}

RasterizerBuilder & RasterizerBuilder::DepthBiasSlopeFactor(float slopeFactor)
{
    depthBiasSlopeFactor = slopeFactor;
    return *this;
}

RasterizerBuilder& RasterizerBuilder::Reset()
{
    depthClamp = false;
    lineWidth = 1.0f;
    polygonMode = VK_POLYGON_MODE_FILL;
    cullMode = VK_CULL_MODE_BACK_BIT;
    frontFace = VK_FRONT_FACE_CLOCKWISE;
    depthBiasClamp = false;
    depthBiasConstantFactor = 0.0f;
    depthBiasClamp = 0.0f;
    depthBiasSlopeFactor = 0.0f;

    return *this;
}

VkPipelineRasterizationStateCreateInfo RasterizerBuilder::Build() const
{
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = depthClamp ? VK_TRUE : VK_FALSE;
    rasterizer.polygonMode = polygonMode;
    rasterizer.lineWidth = lineWidth;
    rasterizer.cullMode = cullMode;
    rasterizer.frontFace = frontFace;
    rasterizer.depthBiasEnable = depthBias;
    rasterizer.depthBiasEnable = depthBias ? VK_TRUE : VK_FALSE;
    rasterizer.depthBiasConstantFactor = depthBiasConstantFactor; 
    rasterizer.depthBiasClamp = depthBiasClamp;                          
    rasterizer.depthBiasSlopeFactor = depthBiasSlopeFactor;                

    return rasterizer;
}
