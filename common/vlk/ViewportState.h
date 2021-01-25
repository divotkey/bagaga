// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// C++ Standard Library includes
#include <vector>

// Vulkan includes
#include <vulkan/vulkan.h>

// Forward declaration
class SwapChain;

/**
 * Utility class used to build Vulkan viewport structures.
 */
class ViewportBuilder {
public:

    /**
     * Constructor.
     */
    ViewportBuilder();

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    ViewportBuilder & Reset();

    /**
     * Specifies the minimum value for depth in the framebuffer.
     * 
     * @param minDepth  the minimum depth value
     * @return reference to this builder used for method chaining
     * @throws std::invalid_argument in case the min depth is less than zero
     */
    ViewportBuilder & MinDepth(float minDepth);

    /**
     * Specifies the minimum value for depth in the framebuffer.
     * 
     * @param maxDepth  the maximum depth value
     * @return reference to this builder used for method chaining
     * @throws std::invalid_argument in case the min depth is greater than one
     */
    ViewportBuilder & MaxDepth(float maxDepth);

    /**
     * Specifies the minimum and maximum value for depth in the framebuffer.
     * 
     * @param minDepth  the minimum depth value
     * @param maxDepth  the maximum depth value
     * @return reference to this builder used for method chaining
     * @throws std::invalid_argument in case the values are out of range [0, 1]
     */
    ViewportBuilder & Depth(float minDepth, float maxDepth) {
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
    ViewportBuilder & Width(float width);

    /**
     * Specifies the height of the viewport;
     * 
     * @param height
     * @return reference to this builder used for method chaining
     * @throws std::invalid_argument in case the height is less or equal zero
     */
    ViewportBuilder & Height(float height);    

    /**
     * Specifies the width and height of the viewport;
     * 
     * @param height
     * @param width
     * @return reference to this builder used for method chaining
     * @throws std::invalid_argument in case the width or height is less or equal zero
     */
    ViewportBuilder & Size(float width, float height)
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
    ViewportBuilder & ChoosePositionAndSize(const SwapChain & swapChain);

    /**
     * Specifies the width and height of the viewport;
     * 
     * @param extend    defines width and heihgt
     * @return reference to this builder used for method chaining
     * @throws std::invalid_argument in case the width or height are zero
     */
    ViewportBuilder & Size(const VkExtent2D & extend)
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
    ViewportBuilder & StartX(float x);

    /**
     * Sets the y-coordinate of the upper left corner of the viewport
     * 
     * @param y the y-coordinate of the upper left corner
     * @return reference to this builder used for method chaining
     * @throws std::invalid_argument in case the coordinage is less than zero
     */
    ViewportBuilder & StartY(float y);

    /**
     * Builds the viewport state structure according to the current configuration.
     * 
     * @return the viewport state structure
     * @throws std::logic_error in case the current configuration is invalid
     */
    VkViewport Build() const;

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

/**
 * Wraps Vulkan VkPipelineViewportStateCreateInfo structure and
 * containts the corresponding VkViewport and VkRect2D 
 * (scissors) structures as well.
 */
class ViewportStateInfo {
public:

    /**
     * Constructor.
     * 
     * The viewport state info structure will be completed by this construtor,
     * according to the content of viewport and scissors parameter.
     * 
     * @param info  the viewport state info structure
     * @param viewports the viewports of the viewport state
     * @param scissors the scissors of the viewport state
     * @throws std::invalid_argument in case the parameters are invalid
     */
    ViewportStateInfo(
        const VkPipelineViewportStateCreateInfo & info, 
        const std::vector<VkViewport> & viewports,
        const std::vector<VkRect2D>  &scissors
        );

    /**
     * Copy constructor.
     * 
     * @param o the other instance
     */
    ViewportStateInfo(const ViewportStateInfo & o);

    /**
     * Returns the viewport state structure.
     * 
     * @return the viewport state create info structure
     */
    const VkPipelineViewportStateCreateInfo & GetInfo() const {
        return viewportState;
    }

    /**
     * Copy assignment operator.
     * 
     * @param rhs   the right hand side
     * @return reference to this instance
     */
    ViewportStateInfo & operator= (const ViewportStateInfo & rhs);

    /**
     * Implicit conversion to Vulkan VkPipelineViewportStateCreateInfo.
     */
    operator VkPipelineViewportStateCreateInfo() const { 
        return GetInfo();
    }

    /**
     * Implicit conversion to pointer to Vulkan VkPipelineViewportStateCreateInfo.
     */
    operator const VkPipelineViewportStateCreateInfo*() const { 
        return &GetInfo();
    }

private:
    /** The viewport state info structure. */
    VkPipelineViewportStateCreateInfo viewportState;

    /** The viewports of this viewport state. */
    std::vector<VkViewport> viewports;

    /** The viewports of this viewport state. */
    std::vector<VkRect2D> scissors;


    /**
     * Updates the viewport state structure.
     */
    void UpdateAndValidate();
};

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
     * Adds a viewport to the viewport state to build.
     * 
     * @param viewport  the viewport to add
     * @return reference to this builder used for method chaining
     */
    ViewportStateBuilder & AddViewport(const VkViewport & viewport);

    /**
     * Adds a viewport to the viewport state to build.
     * 
     * @param viewport  the viewport to add
     * @param scissor   the scissor rectangle for the viewport
     * @return reference to this builder used for method chaining
     */
    ViewportStateBuilder & AddViewport(const VkViewport & viewport, const VkRect2D & scissor);

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    ViewportStateBuilder & Reset();

    /**
     * Builds the viewport state structure according to the current configuration.
     * 
     * @return the viewport info instance
     * @throws std::logic_error in case the current configuration is invalid
     */
    ViewportStateInfo Build() const;

private:
    /** The viewports of this viewport state. */
    std::vector<VkViewport> viewports;

    /** The viewports of this viewport state. */
    std::vector<VkRect2D> scissors;
};