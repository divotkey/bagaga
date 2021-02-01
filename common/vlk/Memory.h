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
class PhysicalDevice;

/**
 * Wraps Vulkan device memory objects.
 */
class Memory {
public:

    /**
     * Deleted copy constructor.
     */
    Memory(const Memory &) = delete;

    /**
     * Deleted assignment operator.
     */
    Memory & operator =(const Memory &) = delete;

    /**
     * Destructor.
     */
    virtual ~Memory();

    /**
     * Returns the allocated memory size in bytes.
     * 
     * @return the size in bytes;
     */
    size_t GetSize() const;

    /**
     * Writes the specified data to this memory object.
     * 
     * @param data      the data to write into this memory object
     * @param offset    the offset within the memory object to start writing
     * @throws std::logic_error in case this memory object is already mapped
     * @throws std::runtime_error in case this memory object could not be mapped
     */
    void WriteData(const std::vector<unsigned char> data, size_t offset = 0);


    void WriteData(const unsigned char* src, size_t numBytes, size_t offset = 0);


    /**
     * Maps this memory object to application address space.
     * 
     * @return the application space address of this memory object
     * @throws std::logic_error in case this memory object is already mapped
     * @throws std::runtime_error in case this memory object could not be mapped
     */
    unsigned char* Map() {
        return Map(GetSize(), 0);
    }

    /**
     * Maps this memory object to application address space.
     * 
     * @param size      size of the memory range to map
     * @param offset    the zero-based byte offset from the beginning of the memory object
     * @return the application space address of this memory object
     * @throws std::logic_error in case this memory object is already mapped
     * @throws std::runtime_error in case this memory object could not be mapped
     */
    unsigned char* Map(size_t size, size_t offset = 0);

    /**
     * Ummaps this memory object.
     * 
     * @throws std::logic_error in case this memory object is currently not mapped
     */
    void Unmap();

private:
    /** Handle to the Vulkan device memory. */
    VkDeviceMemory memory;

    /** The logical device this buffer belongs to. */
    std::shared_ptr<LogicalDevice> device;

    /** The allocated memory size in bytes. */
    VkDeviceSize allocationSize;

    /** Indicates that this memory object is currently mapped to application address space. */
    bool mapped;

    /** Receives the application space address in case this object is currently mapped. */
    unsigned char *data;

    /**
     * Constructor.
     * 
     * @param handle    he handle to the Vulkan memory object
     * @param device    the logical device this memory belongs to
     */
    Memory(VkDeviceMemory handle, std::shared_ptr<LogicalDevice> device);

    friend class MemoryBuilder;
};

/**
 * Utility class to create Memory objects.
 */
class MemoryBuilder {
public:

    /**
     * Constructor.
     */
    MemoryBuilder();


    /**
     * Specifies size of the memory to create in bytes.
     * 
     * @param size  the memory size in bytes
     * @return reference to this builder used for method chaining
     */
    MemoryBuilder & AllocationSize(VkDeviceSize size);

    /**
     * Specifies the index of the memory type from the memoryTypes array.
     * 
     * The memoryTypes array is part of teh VkPhysicalDeviceMemoryProperties structure.
     * 
     * @param index the memory type index
     * @return reference to this builder used for method chaining
     */
    MemoryBuilder & MemoryTypeIndex(uint32_t index);

    /**
     * Returns the memory type index of the current configuration.
     * 
     * @return the memory type index
     */
    uint32_t GetMemoryTypeIndex() const {
        return memoryTypeIndex;
    }

    /**
     * Chooses the index of the memory type.
     * 
     * @param device        the physical device to use
     * @param typeFilter    the possible type indices
     * @param properties    required additional properties
     * @return reference to this builder used for method chaining
     * @throws std::runtime_error in case no suitable memory type index could be found
     */
    MemoryBuilder & ChooseMemoryTypeIndex(
        const PhysicalDevice & device, 
        uint32_t typeFilter, 
        VkMemoryPropertyFlags properties);


    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    MemoryBuilder & Reset();
    
     /**
     * Builds the memory object according to the current configuration.
     * 
     * @param device    the logical device to use
     * @return the newly created memory object
     * @throws std::runtime_error in case the memory object could not be created
     */
   std::unique_ptr<Memory> Build(std::shared_ptr<LogicalDevice> device) const;

private:

    /** The size of the allocation in bytes. */
    VkDeviceSize allocationSize;

    /** Index identifying a memory type from the memoryTypes array. */
    uint32_t memoryTypeIndex;


    /**
     * Searches for a suitable the memory type index.
     * 
     * @param memProperties the memory properties of the physical device
     * @param typeFilter    the possible type indices
     * @param properties    required additional properties
     * @throws std::runtime_error in case no suitable memory type index could be found
     */
    uint32_t FindMemoryType(
        const VkPhysicalDeviceMemoryProperties &memProperties, 
        uint32_t typeFilter, 
        const VkMemoryPropertyFlags & properties) const;
};