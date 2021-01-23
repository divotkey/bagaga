// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// C++ Standard Library includes
#include <string>
#include <vector>

// Forward declaration
class PhysicalDevice;


/**
 * Utility class used to log Vulkan specific information using 
 * the SDL logging mechanism.
 */
class SdlVulkanLogger final {
public:

    /**
     * Deleted constructor.
     * 
     * This utility class should not be instantiated.
     */
    SdlVulkanLogger() = delete;

    /**
     * Deleted copy constructor.
     * 
     * This utility class should not be duplicated.
     */    
    SdlVulkanLogger(const SdlVulkanLogger&) = delete;

    /**
     * Deleted copy assignment operator.
     * 
     * This utility class should not be duplicated.
     */    
    SdlVulkanLogger& operator=(const SdlVulkanLogger&) = delete;

    /**
     * Emits a log messages describing the Vulkan version.
     */
    static void LogVulkanApiVersion();

    /**
     * Emits a log messages describing available Vulkan extensions.
     */
    static void LogVulkanExtensions();

    /**
     * Emits a log messages describing available Vulkan layers.
     */
    static void LogVulkanLayers();

    /**
     * Emits a log messages describing available Vulkan device extensions.
     */
    static void LogDeviceExtensions(const PhysicalDevice & device);

private:

    /**
     * Creates a string with comma separates names.
     * 
     * @param names several names e.g. extenions names or layer names
     * @return string with names
     */
    static std::string MakeCsv(const std::vector<std::string> & names);


};