// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// Vulkan includes
#include <vulkan/vulkan.h>

class VertexInputInfoBuilder {
public:

    /**
     * Constructor.
     */ 
    VertexInputInfoBuilder();

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    VertexInputInfoBuilder & Reset();

    VkPipelineVertexInputStateCreateInfo Build();

private:
};