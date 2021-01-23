// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// C++ Standard Library includes
#include <vector>

// Vulkan includes
#include <vulkan/vulkan.h>

/**
 * Utility class used to build dynamic state structures.
 */
class DynamicStateBuilder {
public:

    /**
     * Constructor.
     */
    DynamicStateBuilder();

    /**
     * Adds a dynamic state.
     * 
     * @param state the state to add
     * @return reference to this builder used for method chaining
     * @throws std::logic_error in case the dynamic state has already beed added
     */
    DynamicStateBuilder & AddDynamicState(VkDynamicState state);

    /***
     * Tests whether a dynamic state has already been added.
     * 
     * @param state the dynamic state to look for
     * @return `true` if the dynamic state has already been added
     */
    bool HasDynamicState(VkDynamicState state) const;

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    DynamicStateBuilder& Reset();

    /**
     * Builds the dynamic state structure according to the current configuration.
     * 
     * @return the dynamic state structure
     * @throws std::logic_error in case the current configuration is invalid
     */
    VkPipelineDynamicStateCreateInfo Build() const;

private:
    /** The dynamic states. */
    std::vector<VkDynamicState> dynamicStates;
};