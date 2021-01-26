// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// C++ Standard Library includes
#include <memory>
#include <vector>

// Vulkan includes
#include <vulkan/vulkan.h>

// Forward declaration
class LogicalDevice;
class SwapChain;


/**
 * Wraps a Vulkan framebuffer object.
 */
class Framebuffer final {
public:

    /**
     * Destructor.
     */
    ~Framebuffer();

    /**
     * Deleted copy constructor.
     */
    Framebuffer(const Framebuffer &) = delete;

    /**
     * Deleted assignment operator.
     */
    Framebuffer & operator =(const Framebuffer &) = delete;


private:
    /** The handle of framebuffer object. */
    VkFramebuffer framebuffer;

    /** The logical device this swap chain belongs to. */
    std::shared_ptr<LogicalDevice> device;

    /**
     * Constructor.
     * 
     * @param handle    the handle to the framebuffer object
     * @param device    the physical device this framebuffer object belongs to
     */
    Framebuffer(VkFramebuffer handle, std::shared_ptr<LogicalDevice> device);


    friend class FramebufferBuilder;
};

/**
 * Utility class used to build fremabuffer objects.
 */
class FramebufferBuilder {
public:

    /**
     * Constructor.
     */
    FramebufferBuilder();

    FramebufferBuilder& Renderpass(VkRenderPass renderPass);

    FramebufferBuilder& Flags(VkFramebufferCreateFlags flags);
    FramebufferBuilder& Width(uint32_t w);
    FramebufferBuilder& Height(uint32_t h);
    FramebufferBuilder& Layers(uint32_t l);
    FramebufferBuilder& AddAttachment(VkImageView attachment);
    FramebufferBuilder& AddAttachments(const std::vector<VkImageView> & attachments);

    /**
     * Sets the width and height of the frame buffer to build.
     * 
     * Note: future versions of this method will set the number as layers as well.
     * 
     * @param swapChain the swapChain used to configure the framebuffer
     * @return reference to this builder used for method chaining
     */
    FramebufferBuilder& ChooseDimension(const SwapChain & swapChain);

    FramebufferBuilder& ChooseConfiguration(const SwapChain & swapChain);

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    FramebufferBuilder& Reset();

    /**
     * Builds the framebuffer object according to the current configuration.
     * 
     * @param device    the logical device to use
     * @return the newly created framebuffer object
     * @throws std::runtime_error in case the framebuffer object could not be created
     */
    std::unique_ptr<Framebuffer> Build(std::shared_ptr<LogicalDevice> device);

private:

    /** Is a bitmask of VkFramebufferCreateFlagBits. */
    VkFramebufferCreateFlags flags;

    /** Defines what render passes the framebuffer will be compatible with. */
    VkRenderPass renderPass;

    /** Used as the corresponding attachment in a render pass instance. */
    std::vector<VkImageView> attachments;

    /** The width of the framebuffer to build. */
    uint32_t width;

    /** The height of the framebuffer to build. */
    uint32_t height;

    /** The number of layers of the framebuffer to build. */
    uint32_t layers;

};