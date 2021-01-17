// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

#include <optional>
#include <vulkan/vulkan.h>

class QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    std::optional<uint32_t> computeFamily;

    bool IsComplete() const {
        return 
            graphicsFamily.has_value() 
            && computeFamily.has_value()
            && presentFamily.has_value();
    }
};        

class QueueIndexFinder {
public:

    /**
     * Constructor.
     */
    QueueIndexFinder();

    /**
     * This constructor can be used to create a finder and start a search.
     * 
     * @param device    the physical device to search for queues
     * @param surface   the window surface
     */
    QueueIndexFinder(VkPhysicalDevice device, VkSurfaceKHR surface);

    /**
     * Virtual destructor.
     */
    virtual ~QueueIndexFinder() {}
    
    /**
     * Searches for indices of queue families.
     * 
     * @param device    the physical device to search for queues
     * @param surface   the window surface
     */
    void Search(VkPhysicalDevice device, VkSurfaceKHR surface);

    /**
     * Tests wherher a graphics queue family has been found.
     * 
     * @return `true` if a graphics queue family has been found
     */
    bool HasGraphicsFamily() const;

    /**
     * Returns the index of the graphics queue family.
     * 
     * @return the index of the graphics queue family
     * @throws std::std::logic_error in case no graphics queue has been found
     */
    uint32_t GetGraphicsFamily() const;

    /**
     * Tests wherher a present queue family has been found.
     * 
     * @return `true` if a present queue family has been found
     */
    bool HasPresentFamily() const;

    /**
     * Returns the index of the present queue family.
     * 
     * @return the index of the present queue family
     * @throws std::std::logic_error in case no graphics queue has been found
     */
    uint32_t GetPresentFamily() const;

    /**
     * Tests wherher a compute queue family has been found.
     * 
     * @return `true` if a compute queue family has been found
     */
    uint32_t GetComputeFamily() const;

    /**
     * Returns the index of the compute queue family.
     * 
     * @return the index of the compute queue family
     * @throws std::std::logic_error in case no graphics queue has been found
     */
    bool HasComputeFamily() const;

    /**
     * Tests whether all queue families have been found.
     * 
     * @return `true` if all queue families have been found
     */
    bool HasAllFamilies() const;

private:
    /** The index of the graphics queue family. */
    std::optional<uint32_t> graphicsFamily;

    /** The index of the present queue family. */
    std::optional<uint32_t> presentFamily;

    /** The index of the compute queue family. */
    std::optional<uint32_t> computeFamily;
};