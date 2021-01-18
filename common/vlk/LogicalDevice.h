// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// Standard C++ Library includes
#include <cstdint>
#include <memory>
#include <vector>
#include <string>

// Vulkan includes.
#include <vulkan/vulkan.h>

// Local includes
#include "NameList.h"

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
     * Returns the handle to the logical device.
     * 
     * @return the device handle
     */
    VkDevice GetHandle() const {
        return logicalDevice;
    }

    /**
     * Returns the graphics queue of this logical device.
     * 
     * @return the graphics queue
     */
    VkQueue GetGraphicsQueue() {
        return graphicsQueue;
    }

    /**
     * Return the index of the graphics queue.
     * 
     * @return the index of the graphics queue
     */
    uint32_t GetGraphicsQueueIndex() const {
        return graphicsQueueIdx;
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
     * Return the index of the present queue.
     * 
     * @return the index of the present queue
     */
    uint32_t GetPresentQueueIndex() const {
        return presentQueueIdx;
    }

    /**
     * Returns the compute queue of this logical device.
     * 
     * @return the compute queue
     */
    VkQueue GetComputeQueue() { 
        return computeQueue;
    }

    /**
     * Return the index of the compute queue.
     * 
     * @return the index of the compute queue
     */
    uint32_t GEtComputeQueueIndex() const {
        return computeQueueIdx;
    }

    /**
     * Returns whether the graphics queue and present queue are identical.
     * 
     * @return `true` if graphics and present queue are identical
     */
    bool IsGraphicsAndPresentQueueIdentical() const;

    /**
     * Implicit conversion to Vulkan handle
     */
    operator VkDevice() const { 
        return GetHandle();
    }

private:

    /** The logical device handle. */
    VkDevice logicalDevice;

    /** The vulkan graphics queue of this logical device. */
    VkQueue graphicsQueue;

    /** The vulkan presentation queue of this logical device. */
    VkQueue presentQueue;

    /** The vulkan compute queue of this logical device. */
    VkQueue computeQueue;

    /** The index of the graphics queue of the physical device. */
    uint32_t graphicsQueueIdx;

    /** The index of the present queue of the physical device. */
    uint32_t presentQueueIdx;

    /** The index of the compute queue of the physical device. */
    uint32_t computeQueueIdx;

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

    /**
     * Constructor.
     */
    LogicalDeviceBuilder();

    /**
     * Adds an extension.
     * 
     * @param extensionName the name of the extension
     * @return reference to this builder for method chaining
     * @throws std::logic_error in case the extension has already been added
     */
    LogicalDeviceBuilder & AddDeviceExtension(const std::string & name);

    /**
     * Adds serveral extensions at once.
     * 
     * @param extensionNames    the vector with extension names
     * @return reference to this builder for method chaining
     * @throws std::logic_error in case the extension has already been added
     */
    LogicalDeviceBuilder & AddDeviceExtensions(const std::vector<std::string> & extensionNames);

    /**
     * Adds serveral extensions at once.
     * 
     * @param extensionNames    the vector with extension names
     * @return reference to this builder for method chaining
     * @throws std::logic_error in case the extension has already been added
     */
    LogicalDeviceBuilder & AddDeviceExtensions(const std::vector<const char *> & extensionNames);

    /**
     * Tests whether the extenions has already been added.
     * 
     * @param extensionName the name of the extenions to test
     * @return `true` in case the extension has already been added
     */
    bool HasDeviceExtension(const std::string & name) const;

    /**
     * Adds an layer.
     * 
     * @return reference to this builder for method chaining
     * @throws std::logic_error in case the layer has already been added
     */
    LogicalDeviceBuilder & AddDeviceLayer(const std::string & name);

    /**
     * Adds serveral layers at once.
     * 
     * @param layerNames    the vector with layer names
     * @return reference to this builder for method chaining
     * @throws std::logic_error in case the extension has already been added
     */
    LogicalDeviceBuilder & AddDeviceLayers(const std::vector<std::string> & layerNames);

    /**
     * Adds serveral layers at once.
     * 
     * @param layerNames    the vector with layer names
     * @return reference to this builder for method chaining
     * @throws std::logic_error in case the extension has already been added
     */
    LogicalDeviceBuilder & AddDeviceLayers(const std::vector<const char *> & layerNames);

    /**
     * Tests whether the layer has already been added.
     * 
     * @param extensionName the name of the layer to test
     * @return `true` in case the layer has already been added
     */
    bool HasDeviceLayer(const std::string & name) const;

    /**
     * Resets this builder to its initial state.
     * 
     * @return reference to this builder for method chaining
     */
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
    /** The variable holding the priority for all queues. */
    const static float kQPriority;

    /** The names of the required device extensions. */
    NameList extensionNames;

    /** The names of the required device layers. */
    NameList layerNames;

    /**
     * Creates VkDeviceQueueCreateInfo structure based on required queues.
     * 
     * @return vector with create info structures
     */
    std::vector<VkDeviceQueueCreateInfo> BuildQueueCreateInfos(const QueueIndexFinder & qid);

    VkPhysicalDeviceFeatures BuildDeviceFeatures();
};