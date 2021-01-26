// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// C++ Standard Library includes
#include <memory>
#include <optional>

// Vulkan includes
#include <vulkan/vulkan.h>

// Forward declaration
class CommandBuffer;

/**
 * Wraps a Vulkan command pool.
 */
class CommandPool : public std::enable_shared_from_this<CommandPool> {
public:

    /**
     * Constructor.
     * 
     * @param handle    the handle to the command pool
     * @param device    the physical device this command pool object belongs to
     */
    CommandPool(VkCommandPool handle, std::shared_ptr<LogicalDevice> device);

    /**
     * Destructor.
     */
    ~CommandPool();

    /**
     * Allocates a new command buffer.
     * 
     * @param primary   set to `true` to create primary command buffers `false`
     *                  for secondary command buffers.
     * @return the newly created command buffer
     */
    std::unique_ptr<CommandBuffer> CreateCommandBuffer(bool primary = true);

    /**
     * Allocates a bunch of new command buffers.
     * 
     * @param primary   set to `true` to create primary command buffers `false`
     *                  for secondary command buffers.
     * @param n         the number of command buffers to create
     * @return the newly created command buffers
     */
    std::vector<std::unique_ptr<CommandBuffer>> CreateCommandBuffers(size_t n, bool primary = true);

private:
    /** The handle to the command pool object. */
    VkCommandPool commandPool;

    /** The logical device this swap chain belongs to. */
    std::shared_ptr<LogicalDevice> device;


    /**
     * Frees the specified command buffer.
     * 
     * @param   the command buffer do dealocate
     */
    void Free(CommandBuffer & cmdBuf);


    friend class CommandBuffer;
};

class CommandPoolBuilder {
public:

    /**
     * Constructor.
     */
    CommandPoolBuilder();

    /**
     * Specifies the index of the queue family used by the command pool.
     * 
     * @param index the queue family index
     * @return reference to this builder used for method chaining
     */
    CommandPoolBuilder& QueueFamilyIndex(uint32_t index);

    /**
     * Specifies flags indicating usage behavior for the pool.
     * 
     * @param flags bitmask of VkCommandPoolCreateFlagBits
     * @return reference to this builder used for method chaining
     */
    CommandPoolBuilder& Flags(VkCommandPoolCreateFlags flags);

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    CommandPoolBuilder& Reset();

    /**
     * Builds the command pool object according to the current configuration.
     * 
     * @param device    the logical device to use
     * @return the newly created command pool object
     * @throws std::runtime_error in case the command pool object could not be created
     */
    std::shared_ptr<CommandPool> Build(std::shared_ptr<LogicalDevice> device);

private:
    /** Designates a queue family. */
    std::optional<uint32_t> queueFamilyIndex;

    /** Bitmask of VkCommandPoolCreateFlagBits indicating usage behavior for the pool. */
    VkCommandPoolCreateFlags flags;

    /**
     * Validates the current configuration.
     * 
     * @throws std::logic_error in case the configuration is incomplete or invalid
     */
    void ValidateConfiguration();
};