// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// Vulkan includes
#include <vulkan/vulkan.h>

class InputAssemblyBuilder {
public:

    /**
     * Constructor.
     */ 
    InputAssemblyBuilder();

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    InputAssemblyBuilder & Reset();

    InputAssemblyBuilder & Topology(VkPrimitiveTopology topology);

    InputAssemblyBuilder & PrimitiveRestart(bool enable);

    VkPipelineInputAssemblyStateCreateInfo Build() const;

private:
    /** Describes the kind of geometry to be rendered. */
    VkPrimitiveTopology topology;

    /** Whether to use the primitive restart enable flag. */
    bool primitiveRestart;
};