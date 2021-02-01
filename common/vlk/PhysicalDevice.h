// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// C++ Standard Library includes
#include <vector>
#include <string>
#include <set>

// Vulkan includes
#include <vulkan/vulkan.h>

/**
 * Wraps a logical Vulkan device.
 */
class PhysicalDevice final {
public:

    /**
     * Retrieves all available physical Vulkan devices.
     * 
     * @param instance  the Vulkan instance
     * @return a vector containing all physical Vulkan devices
     * @throws std::runtime_error in case the Vulkan devices coult not be queried
     */
    static std::vector<PhysicalDevice> GetVulkanDevices(VkInstance instance);

    /**
     * Constructor.
     * 
     * @param device    the handle to the physical device
     */
    PhysicalDevice(VkPhysicalDevice device);

    /**
     * Returns the handle to the physical device.
     * 
     * @return the device handle
     */
    VkPhysicalDevice GetHandle() const {
        return device;
    }

    /**
     * Returns the names of available device extensions.
     * 
     * @return vector containing names of device extensions
     * @throws std::runtime_error in case of a problem while querying 
     *      the device extensions
     */
    std::vector<std::string> GetAvailableExtensions() const;

    /**
     * Returns the properties of this device's queue families.
     * 
     * @return vector containing the queue family properties
     */
    std::vector<VkQueueFamilyProperties> GetQueueFamilyProperties() const;

    /**
     * Tests whether this device supports specific device extensions.
     * 
     * This is a convenience method which allows the extension names to be specified
     * as vector of c-strings.
     * 
     * @param extensionNames    the names of the requested device extensions
     */
    bool SupportsExtensions(const std::vector<const char*> & extensionNames) const;

    /**
     * Tests whether this device supports specific device extensions.
     * 
     * This is a convenience method which allows the extension names to be specified
     * as vector of strings.
     * 
     * @param extensionNames    the names of the requested device extensions
     */
    bool SupportsExtensions(const std::vector<std::string>  & extensionNames) const;

    /**
     * Tests whether this device supports specific device extensions.
     * 
     * @param extensionNames    the names of the requested device extensions
     */
    bool SupportsExtensions(std::set<std::string> extensionNames) const;

    /**
     * Retrieves the surface capabilities of this physical device.
     * 
     * @param surface   the window surface
     * @return the surface capabilities
     * @throws std::runtime_error in case the surface capabilities could not be queried
     */
    VkSurfaceCapabilitiesKHR GetSurfaceCapabilities(VkSurfaceKHR surface) const;

    /**
     * Retrieves the supported swapchain format-color space pairs.
     * 
     * @param surface   the window surface
     * @return a vector holding the swapchain format-color space pairs
     * @throws std::runtime_error in case the format-color space pairs could not be queried
     */
    std::vector<VkSurfaceFormatKHR> GetSurfaceFormats(VkSurfaceKHR surface) const;

    /**
     * Retrieves the supported presentation modes for a surface
     * 
     * @param surface   the window surface
     * @return a vector holding the presentation modes
     * @throws std::runtime_error in case the presentation modes could not be queried
     */
    std::vector<VkPresentModeKHR> GetPresentModes(VkSurfaceKHR surface) const;

    /**
     * Retrieves the memory properties of this physical device.
     * 
     * @return the memory properties
     */
    VkPhysicalDeviceMemoryProperties GetMemoryProperties() const;

    /**
     * Returns the type of this physical device.
     * 
     * @return the device type;
     */
    VkPhysicalDeviceType GetDeviceType() const {
        return GetProperties().deviceType;
    }

    /**
     * Returns whether this physicsl device is an integrated GPU.
     * 
     * If this device is a discrete GPU it is typically one embedded in or
     * tightly coupled with the host.
     * 
     * @return `true` if this device is an integraged GPU
     */
    bool IsIntegratedGPU() const {
        return GetDeviceType() == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;
    }

    /**
     * Returns whether this physicsl device is a discrete GPU.
     * 
     * If this device is a discrete GPU it is typically a separate processor
     * connected to the host via an interlink.
     * 
     * @return `true` if this device is a discrete GPU
     */
    bool IsDiscreteGPU() const {
        return GetDeviceType() == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
    }

    /**
     * Returns maximum image dimension created with VK_IMAGE_TYPE_2D.
     * 
     * @return the maximum image dimension
     */
    uint32_t GetMaxImageDimension2D() const {
        return GetLimits().maxImageDimension2D;
    }

    /**
     * Return the name of this pyhsicsl device.
     * 
     * @return the device name
     */
    const char* GetName() const {
        return GetProperties().deviceName;
    }

    /**
     * Returns the device Properties.
     * 
     * @return the device properties.
     */
    const VkPhysicalDeviceProperties & GetProperties() const;

    /**
     * Returns the limits of this physical device.
     * 
     * @return the device limits
     */
    const VkPhysicalDeviceLimits & GetLimits() const {
        return GetProperties().limits;
    }

    /**
     * Implicit conversion to Vulkan handle
     */
    operator VkPhysicalDevice() const { 
        return GetHandle();
    }

private:
    /** The handle to the physical device. */
    VkPhysicalDevice device;

    /** The device properties.*/
    VkPhysicalDeviceProperties properties;
};