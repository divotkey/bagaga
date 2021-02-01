// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
#include <stdexcept>
#include <cassert>
#include <cstdint>

// Local includes
#include "PhysicalDevice.h"

using namespace std;

vector<PhysicalDevice> PhysicalDevice::GetVulkanDevices(VkInstance instance)
{
    assert(instance);

    vector<PhysicalDevice> result;

    uint32_t deviceCount = 0;
    VkResult res = vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (res != VK_SUCCESS)
    {
        throw runtime_error("Unable to query number of physical Vulkan devices, error code " 
            + to_string(res));
    }

    vector<VkPhysicalDevice> devices(deviceCount);
    res = vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    if (res != VK_SUCCESS)
    {
        throw runtime_error("Unable to query physical Vulkan devices " 
            + to_string(res));
    }

    for (auto device : devices) {
        result.push_back(device);
    }

    return result;
}

PhysicalDevice::PhysicalDevice(VkPhysicalDevice device)
    : device(device)
{
    assert(device);
    vkGetPhysicalDeviceProperties(device, &properties);     
}

bool PhysicalDevice::SupportsExtensions(const vector<const char*> & names) const
{
    return SupportsExtensions(set<string>(names.begin(), names.end()));
}

bool PhysicalDevice::SupportsExtensions(const vector<string>  & names) const
{
    return SupportsExtensions(set<string>(names.begin(), names.end()));
}

bool PhysicalDevice::SupportsExtensions(set<string> names) const
{
    for (const auto & ext : GetAvailableExtensions()) {
        names.erase(ext);
    }

    return names.empty();
}

vector<string> PhysicalDevice::GetAvailableExtensions() const
{
    uint32_t extensionCount;

    VkResult res = vkEnumerateDeviceExtensionProperties(
        device, 
        nullptr, 
        &extensionCount, 
        nullptr
        );

    if (res != VK_SUCCESS) {
        throw runtime_error("Unable to query number of device extension properties, error code " 
            + to_string(res));
    }

    vector<VkExtensionProperties> extensionProperties(extensionCount);
    res = vkEnumerateDeviceExtensionProperties(
        device,
        nullptr, 
        &extensionCount, 
        extensionProperties.data()
        );

    if (res != VK_SUCCESS)
    {
        throw runtime_error("Unable to query device extension properties, error code " 
            + to_string(res));
    }

    vector<string> result;
    for (auto & prop : extensionProperties) {
        if (*prop.extensionName) {
            result.push_back(prop.extensionName);
        }
    }

    return result;
}

VkSurfaceCapabilitiesKHR PhysicalDevice::GetSurfaceCapabilities(VkSurfaceKHR surface) const
{
    VkSurfaceCapabilitiesKHR capabilities;

    VkResult res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &capabilities);
    if (res != VK_SUCCESS)
    {
        throw runtime_error("Unable to query surface capabilities of physical device, error code " 
            + to_string(res));
    }

    return capabilities;
}

vector<VkSurfaceFormatKHR> PhysicalDevice::GetSurfaceFormats(VkSurfaceKHR surface) const
{
    uint32_t formatCount;
    VkResult res = vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (res != VK_SUCCESS)
    {
        throw runtime_error("Unable to query number of swapchain format-color space pairs of physical device, error code " 
            + to_string(res));
    }

    vector<VkSurfaceFormatKHR> formats(formatCount);
    res = vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, formats.data());

    if (res != VK_SUCCESS)
    {
        throw runtime_error("Unable to query swapchain format-color space pairs of physical device, error code " 
            + to_string(res));
    }

    return formats;
}

vector<VkPresentModeKHR> PhysicalDevice::GetPresentModes(VkSurfaceKHR surface) const
{
    uint32_t presentModeCount;
    VkResult res = vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (res != VK_SUCCESS)
    {
        throw runtime_error("Unable to query number of presentation modes of physical device, error code " 
            + to_string(res));
    }

    vector<VkPresentModeKHR> modes(presentModeCount);
    res = vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, modes.data());

    if (res != VK_SUCCESS)
    {
        throw runtime_error("Unable to query presentation modes of physical device, error code " 
            + to_string(res));
    }

    return modes;
}

VkPhysicalDeviceMemoryProperties PhysicalDevice::GetMemoryProperties() const
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(device, &memProperties);    

    return memProperties;
}

vector<VkQueueFamilyProperties> PhysicalDevice::GetQueueFamilyProperties() const
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());


    return queueFamilies;
}

const VkPhysicalDeviceProperties & PhysicalDevice::GetProperties() const
{
    return properties;
}

