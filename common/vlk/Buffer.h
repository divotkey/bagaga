// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// C++ Standard Library includes
#include <cstdint>
#include <memory>
#include <vector>

// Vulkan includes
#include <vulkan/vulkan.h>

// Forward declaration
class LogicalDevice;
class Memory;


/**
 * Wrapper class for Vulkan buffers objects.
 */
class Buffer {
public:

    /**
     * Deleted copy constructor.
     */
    Buffer(const Buffer &) = delete;

    /**
     * Deleted assignment operator.
     */
    Buffer & operator =(const Buffer &) = delete;

    /**
     * Destructor.
     */
    virtual ~Buffer();

    /**
     * Binds device memory to this buffer object.
     * 
     * @param memory    the device memory
     * @param offset    is the start offset of the region of memory
     */
    void BindMemory(std::shared_ptr<Memory> memory, size_t offset = 0);

    /**
     * Return the memory object bound to this buffer.
     * 
     * @param the memory object or nullptr if no memory has been bound to this buffer
     */
    std::shared_ptr<Memory> GetMemory() {
        return memory;
    }

    /**
     * Retrieves the memory requirements of this buffer.
     * 
     * @return struct containing the memory requirements of this buffer
     */
    VkMemoryRequirements GetMemoryRequirements() const;

    /**
     * Returns the handle to the buffer object.
     * 
     * @return the buffer object handle
     */
    VkBuffer GetHandle() const {
        return buffer;
    }

    /**
     * Implicit conversion to Vulkan handle
     */
    operator VkBuffer() const { 
        return GetHandle();
    }

private:
    /** Handle to the Vulkan buffer object. */
    VkBuffer buffer;

    /** The logical device this buffer belongs to. */
    std::shared_ptr<LogicalDevice> device;

    /** The device memory bound to this buffer. */
    std::shared_ptr<Memory> memory;

    /**
     * Constructor.
     * 
     * @param handle    he handle to the Vulkan buffer object
     * @param device    the logical device this buffer belongs to
     */
    Buffer(VkBuffer handle, std::shared_ptr<LogicalDevice> device);

    friend class BufferBuilder;
};

/**
 * Utility class to create Buffer objects.
 */
class BufferBuilder {
public:

    /**
     * Constructor.
     */
    BufferBuilder();

    /**
     * Specifies additional parameter of a buffer.
     * 
     * @param   flags the bitmask
     * @return reference to this builder used for method chaining
     */
    BufferBuilder & Flags(VkBufferCreateFlags flags);

    /**
     * Specifies the size in bytes of the buffer to be created.
     * 
     * @param size  the size in bytes
     * @return reference to this builder used for method chaining
     */
    BufferBuilder & Size(VkDeviceSize size);


    /**
     * Specifies allowed usages of the buffer. 
     * 
     * @param usage the bitmask
     * @return reference to this builder used for method chaining
     */
    BufferBuilder & Usage(VkBufferUsageFlags usage);

    /**
     * Specifies the sharing mode of the buffer.
     * 
     * @param sharingMode the sharing mode
     * @return reference to this builder used for method chaining
     */
    BufferBuilder & SharingMode(VkSharingMode sharingMode);

    /**
     * Adds a queue family that will access this buffer.
     * 
     * @param index the index of the queue family to add
     * @return reference to this builder used for method chaining
     */
    BufferBuilder & AddQueueFamily(uint32_t index);

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    BufferBuilder & Reset();
    
     /**
     * Builds the buffer object according to the current configuration.
     * 
     * @param device    the logical device to use
     * @return the newly created buffer object
     * @throws std::runtime_error in case the buffer object could not be created
     */
   std::unique_ptr<Buffer> Build(std::shared_ptr<LogicalDevice> device) const;

private:
    /** Specifies additional parameters of the buffer. */
    VkBufferCreateFlags flags;

    /** The size in bytes of the buffer to be created. */
    VkDeviceSize size;

    /** Bitmask specifying allowed usages of the buffer. */
    VkBufferUsageFlags usage;
    
    /** Specifies the sharing mode of the buffer. */
    VkSharingMode sharingMode;

    /** List of queue families that will access this buffer. */
    std::vector<uint32_t> queueFamilyIndices;
};