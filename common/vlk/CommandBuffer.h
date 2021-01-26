// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// C++ Standard Library includes
#include <memory>

// Vulkan includes
#include <vulkan/vulkan.h>

// Forward declaration
class CommandPool;

/**
 * Wraps Vulkan command Buffer objects.
 */
class CommandBuffer final {
public:

    /**
     * Destructor.
     */
    ~CommandBuffer();

    /**
     * Deleted copy constructor.
     */
    CommandBuffer(const CommandBuffer &) = delete;

    /**
     * Deleted assignment operator.
     */
    CommandBuffer & operator =(const CommandBuffer &) = delete;

    /**
     * Begins recording of commands.
     */
    void Begin();

    /**
     * Ends recording of commands.
     */
    void End();

    /**
     * Returns the usage flags of this command buffer.
     * 
     * @return the usage flags
     */
    VkCommandBufferUsageFlags GetFlags() const {
        return flags;
    }

    /**
     * Sets the usage flags of this command buffer.
     * 
     * @param flags the usage flags
     */
    void SetFlags(VkCommandBufferUsageFlags flags)
    {
        flags = flags;
    }

    /**
     * Clears the behavior flags for this command buffer.
     */
    void ClearFlags() {
        flags = 0;
    }

    /**
     * Specifies that each recording of the command buffer will only be submitted once.
     */
    void SetOneTimeSubmit() {
        flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    }

    /**
     * Specifies that a secondary command buffer is considered to be entirely inside a render pass.
     * 
     * If this is a primary command buffer, then this bit is ignored.
     */
    void SetRenderpassContinue() {
        flags |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    }

    /**
     * Specifies that a command buffer can be resubmitted to a queue while it is in the pending state.
     * 
     * It also indicates that it can be recorded into multiple primary command buffers.
     */
    void SetSimultaneousUse() {
        flags |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    }

    /**
     * Records a command to begin a new render pass.
     * 
     * @param renderPass    handle to the render pass object
     * @param framebuffer   handle to the frame buffer object
     * @param inline        set to `true` if the contents of the subpass will be
     *                      recorded inline in the primary command buffer  
     */
    void BeginRenderPass(VkRenderPass renderPass, VkFramebuffer framebuffer, bool inlineSubpass = true);

    /**
     * Records a command to end a render pass.
     */
    void EndRenderPass();

    /**
     * Specifies the render area that is affected by the render pass instance.
     * 
     * This specifies the render area that is affected by the render
     * pass instance. The effects of attachment load, store and multisample
     * resolve operations are restricted to the pixels whose x and y
     * coordinates fall within the render area on all attachments. The
     * render area extends to all layers of framebuffer. The application
     * must ensure (using scissor if necessary) that all rendering is
     * contained within the render area.
     * 
     * @param w     the width of the render area
     * @param h     the height of the render area
     * @param ox    the x-coordinate of the render area's offset
     * @param ox    the y-coordinate of the render area's offset
     */
    void SetRenderArea(uint32_t w, uint32_t h, int32_t ox, int32_t oy);

    /**
     * Records a non-indexed draw.
     * 
     * @param vertexCount   the number of vertices to draw  
     * @param instanceCount the number of instances to draw
     * @param firstVertex   the index of the first vertex to draw
     * @param firstInstance instance ID of the first instance to draw
     */
    void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);

    /**
     * Returns the Vulkan handle of this command buffer object.
     * 
     * @return the handle
     */
    VkCommandBuffer GetHandle() const {
        return commandBuffer;
    }

    /**
     * Implicit conversion to Vulkan handle
     */
    operator VkCommandBuffer() const { 
        return GetHandle();
    }

private:
    /** Handle to the command buffer object. */
    VkCommandBuffer commandBuffer;

    /** The command pool used to allocate this command buffer. */
    std::weak_ptr<CommandPool> pool;

    /** Bitmask specifying usage behavior for the command buffer. */
    VkCommandBufferUsageFlags flags;

    /** The render area that is affected by the render pass instance. */
    VkRect2D renderArea;

    /**
     * Constructor.
     * 
     * @param handle    the handle to the command buffer object
     * @param pool      the command pool used to allocate this command buffer
     */
    CommandBuffer(VkCommandBuffer handle, std::shared_ptr<CommandPool> pool);

    friend class CommandPool;
};