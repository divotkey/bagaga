// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// Vulkan includes
#include <vulkan/vulkan.h>

// Vulkan includes
#include <vulkan/vulkan.h>

/**
 * Utility class used to build rasterizer structures.
 */
class RasterizerBuilder {
public:

    RasterizerBuilder();

    RasterizerBuilder& Reset();

    VkPipelineRasterizationStateCreateInfo Build();

private:
};