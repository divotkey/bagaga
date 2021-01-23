// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// Vulkan includes
#include <vulkan/vulkan.h>

/**
 * Utility class used to build deth and stencil state structures.
 */
class DepthStencilStateBuilder {
public:

    /**
     * Constructor.
     */
    DepthStencilStateBuilder();

    DepthStencilStateBuilder& DepthTest(bool value);
    DepthStencilStateBuilder& DepthWrite(bool value);
    DepthStencilStateBuilder& DepthCompareOp(VkCompareOp compareOp);
    DepthStencilStateBuilder& DepthBoundsTest(bool value);
    DepthStencilStateBuilder& StencilTest(bool value);
    DepthStencilStateBuilder& Front(VkStencilOpState opState);
    DepthStencilStateBuilder& Back(VkStencilOpState opState);
    DepthStencilStateBuilder& MinDepthBounds(float bounds);
    DepthStencilStateBuilder& MaxDepthBounds(float bounds);

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    DepthStencilStateBuilder& Reset();

    /**
     * Builds the deth and stencil state structure according to the current configuration.
     * 
     * @return the deth and stencil state structure
     * @throws std::logic_error in case the current configuration is invalid
     */
    VkPipelineDepthStencilStateCreateInfo Build() const;

private:

    /** Controls whether depth testing is enabled. */
    bool depthTest;

    /** Controls whether depth writes are enabled when depthTest is enabled. */
    bool depthWrite;

    /** Is the comparison operator used in the depth test. */
    VkCompareOp depthCompareOp;

    /** Controls whether depth bounds testing is enabled. */
    bool depthBoundsTest;

    /** Controls whether stencil testing is enabled. */
    bool stencilTest;

    /* Control the parameters of the stencil test. */
    VkStencilOpState front;

    /* Control the parameters of the stencil test. */
    VkStencilOpState back;

    /** Is the minimum depth bound used in the depth bounds test. */
    float minDepthBounds;

    /** Is the maximum depth bound used in the depth bounds test. */
    float maxDepthBounds;
};