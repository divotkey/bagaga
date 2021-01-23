// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// Vulkan includes
#include <vulkan/vulkan.h>

/**
 * Utility class used to build viewport state structures.
 */
class ViewportStateBuilder {
public:

    /**
     * Constructor.
     */
    ViewportStateBuilder();

    /**
     * Specifies the minimum value for depth in the framebuffer.
     * 
     * @param minDepth  the minimum depth value
     * @return reference to this builder used for method chaining
     * @throws std::invalid_argument in case the min depth is less than zero
     */
    ViewportStateBuilder & MinDepth(float minDepth);

    /**
     * Specifies the minimum value for depth in the framebuffer.
     * 
     * @param maxDepth  the maximum depth value
     * @return reference to this builder used for method chaining
     * @throws std::invalid_argument in case the min depth is greater than one
     */
    ViewportStateBuilder & MaxDepth(float maxDepth);

    /**
     * Specifies the minimum and maximum value for depth in the framebuffer.
     * 
     * @param minDepth  the minimum depth value
     * @param maxDepth  the maximum depth value
     * @return reference to this builder used for method chaining
     * @throws std::invalid_argument in case the values are out of range [0, 1]
     */
    ViewportStateBuilder & Depth(float minDepth, float maxDepth) {
        MinDepth(minDepth);
        MaxDepth(maxDepth);
        return*this;
    }

    /**
     * Specifies the width of the viewport;
     * 
     * @param width
     * @return reference to this builder used for method chaining
     * @throws std::invalid_argument in case the width is less or equal zero
     */
    ViewportStateBuilder & Width(float width);

    /**
     * Specifies the height of the viewport;
     * 
     * @param height
     * @return reference to this builder used for method chaining
     * @throws std::invalid_argument in case the height is less or equal zero
     */
    ViewportStateBuilder & Height(float height);    

    /**
     * Specifies the width and height of the viewport;
     * 
     * @param height
     * @param width
     * @return reference to this builder used for method chaining
     * @throws std::invalid_argument in case the width or height is less or equal zero
     */
    ViewportStateBuilder & Size(float width, float height)
    {
        Width(width);
        Height(height);
        return *this;
    }

    /**
     * Chooses an upper left corner and dimension based on the given swap chain.
     * 
     * @param swapChain the swap chain
     * @return reference to this builder used for method chaining
     */
    ViewportStateBuilder & ChoosePositionAndSize(const SwapChain & swapChain);

    /**
     * Specifies the width and height of the viewport;
     * 
     * @param extend    defines width and heihgt
     * @return reference to this builder used for method chaining
     * @throws std::invalid_argument in case the width or height are zero
     */
    ViewportStateBuilder & Size(const VkExtent2D & extend)
    {
        Width(static_cast<float>(extend.width));
        Height(static_cast<float>(extend.height));
        return *this;
    }

    /**
     * Sets the x-coordinate of the upper left corner of the viewport
     * 
     * @param x the x-coordinate of the upper left corner
     * @return reference to this builder used for method chaining
     * @throws std::invalid_argument in case the coordinage is less than zero
     */
    ViewportStateBuilder & StartX(float x);

    /**
     * Sets the y-coordinate of the upper left corner of the viewport
     * 
     * @param y the y-coordinate of the upper left corner
     * @return reference to this builder used for method chaining
     * @throws std::invalid_argument in case the coordinage is less than zero
     */
    ViewportStateBuilder & StartY(float y);

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    ViewportStateBuilder & Reset();

    /**
     * Builds the viewport state structure according to the current configuration.
     * 
     * @return the viewport state structure
     * @throws std::logic_error in case the current configuration is invalid
     */
    VkPipelineViewportStateCreateInfo Build() const;

private:
    /** The minimum value for the depth of the framebuffer. */
    float minDepth;

    /** The maximum value for the depth of the framebuffer. */
    float maxDepth;

    /** The x-coordinate of the viewport upper left corner of the viewport. */
    float startX;

    /** The y-coordinate of the viewport upper left corner of the viewport. */
    float startY;

    /** The width of the viewport. */
    float width;

    /** The height of the viewport. */
    float height;


    /**
     * Validates the current configuration.
     * 
     * @throws std::logic_error in case the configuration is incomplete or invalid
     */
    void ValidateConfiguration() const;
};