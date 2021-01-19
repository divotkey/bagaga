// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// Standard C++ Library includes
#include <memory>

// Vulkan includes
#include <vulkan/vulkan.h>

// Forward declaration
class LogicalDevice;
class SwapChain;

/**
 * Wraps a Vulkan render pass object.
 */
class RenderPass final {
public:

    /**
     * Constructor.
     * 
     * @param handle    handle to the render pass object
     * @param device    the logical vulkan device used to create this object
     */
    RenderPass(VkRenderPass handle, std::shared_ptr<LogicalDevice> device);

    /**
     * Destructor.
     */
    ~RenderPass();

private:
    /** Handle to the render pass object. */
    VkRenderPass renderPass;

    /** The logical Vulkan device used to create this render pass object. */
    std::shared_ptr<LogicalDevice> device;
};

class RenderPassBuilder {
public:

    /**
     * Constructor.
     */
    RenderPassBuilder();

    /**
     * Specifies the format of the color attachment.
     * 
     * @param format    the format of the color attachment
     * @return reference to this builder for method chaining
     */
    RenderPassBuilder& ColorAttachmentFormat(VkFormat format);

    /**
     * Chooses the color attachment format based to the geiven swap chain.
     * 
     * @param swapChain the swap chain
     * @return reference to this builder for method chaining
     */
    RenderPassBuilder& CooseColorAttachmentFormat(SwapChain & swapChain);

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    RenderPassBuilder& Reset();

    /**
     * Builds the swap chain according to the current configuration.
     * 
     * @param device    the logical device to use
     * @return the newly created render pass object
     * @throws std::runtime_error in case render pass object could not be created
     */
    std::unique_ptr<RenderPass> Build(std::shared_ptr<LogicalDevice> device);

private:
    /** The format of the color attachment. */
    VkFormat colorAttachmentFormat;

    /** The number of samples. */
    VkSampleCountFlagBits sampleCount;

};