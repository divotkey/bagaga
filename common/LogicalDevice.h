// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <string>

#include <vulkan/vulkan.h>

// Forward declaration.
class QueueIndexFinder;

/**
 * Wraps a logical Vulkan device.
 */
class LogicalDevice {
public:

    /**
     * Virtual destructor.
     */
    virtual ~LogicalDevice();

    /**
     * Deleted copy constructor.
     * 
     * A logical device must not be duplicated
     */    
    LogicalDevice(const LogicalDevice&) = delete;

    /**
     * Deleted copy assignment operator.
     * 
     * A logical device must not be duplicated
     */    
    LogicalDevice& operator=(const LogicalDevice&) = delete;

    /**
     * Returns the graphics queue of this logical device.
     * 
     * @return the graphics queue
     */
    VkQueue GetGraphicsQueue() {
        return graphicsQueue;
    }

    /**
     * Returns the present queue of this logical device.
     * 
     * @return the present queue
     */
    VkQueue GetPresentQueue() { 
        return presentQueue;
    }

    /**
     * Returns the compute queue of this logical device.
     * 
     * @return the compute queue
     */
    VkQueue GetComputeQueue() { 
        return computeQueue;
    }

private:

    /** The logical device. */
    VkDevice logicalDevice;

    /** The vulkan graphics queue of this logical device. */
    VkQueue graphicsQueue;

    /** The vulkan presentation queue of this logical device. */
    VkQueue presentQueue;

    /** The vulkan compute queue of this logical device. */
    VkQueue computeQueue;


    /**
     * Private constructor.
     */
    LogicalDevice();

    friend class LogicalDeviceBuilder;
};


/**
 * Builds logical Vulkan devices.
 * 
 * @TOTO add priorities to queues.
 */
class LogicalDeviceBuilder {
public:

    LogicalDeviceBuilder();

    LogicalDeviceBuilder & AddDeviceExtension(const std::string & name);
    bool HasDeviceExtension(const std::string & name) const;
    LogicalDeviceBuilder & AddDeviceLayer(const std::string & name);
    bool HasDeviceLayer(const std::string & name) const;


    LogicalDeviceBuilder & Reset();

    /**
     * Builds a logical device based on the current configuration.
     * 
     * @param device    the physical device to use
     * @param surface   the window surface
     * @return the newly created logical device
     */
    std::unique_ptr<LogicalDevice> Build(VkPhysicalDevice device, VkSurfaceKHR surface);

private:
    /** The names of the required device extensions. */
    std::vector<std::string> extensionNames;

    /** The names of the required device layers. */
    std::vector<std::string> layerNames;


    /**
     * Creates VkDeviceQueueCreateInfo structure based on required queues.
     * 
     * @return vector with create info structures
     */
    std::vector<VkDeviceQueueCreateInfo> BuildQueueCreateInfos(const QueueIndexFinder & qid);

    VkPhysicalDeviceFeatures BuildDeviceFeatures();
    std::vector<const char*> GetPointers(const std::vector<std::string> & names) const;

};