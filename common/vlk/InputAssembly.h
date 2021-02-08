// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// Vulkan includes
#include <vulkan/vulkan.h>


/**
 * Utility class used to build Vulkan input assembly structures.
 */
class InputAssemblyBuilder {
public:

    /**
     * Constructor.
     */ 
    InputAssemblyBuilder();

    /**
     * Specifies the the primitive topology.
     * 
     * @param topology  the topology of the input primitives
     * @return reference to this builder used for method chaining
     */
    InputAssemblyBuilder & Topology(VkPrimitiveTopology topology);

    /**
     * Controls whether a special vertex index value is treated as restarting the assembly of primitives.
     * 
     * @param enable    set to `true` to enable primitive restart
     * @return reference to this builder used for method chaining
     */
    InputAssemblyBuilder & PrimitiveRestart(bool enable);

    /**
     * Builds the input assembly structure according to the current configuration.
     * 
     * @return the newly created input assembly structure
     */
    VkPipelineInputAssemblyStateCreateInfo Build() const;

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    InputAssemblyBuilder & Reset();


private:
    /** Describes the kind of geometry to be rendered. */
    VkPrimitiveTopology topology;

    /** Whether to use the primitive restart enable flag. */
    bool primitiveRestart;
};