// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// Standard C++ Library includes
#include <stdexcept>
#include <cassert>
#include <cstdint>

#include "PhysicalDevice.h"


std::vector<PhysicalDevice> PhysicalDevice::GetVulkanDevices(VkInstance instance)
{
    assert(instance);

    std::vector<PhysicalDevice> result;

    uint32_t deviceCount = 0;
    VkResult res = vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (res != VK_SUCCESS)
    {
        throw std::runtime_error("Unable to query number of physical Vulkan devices, error code " 
            + std::to_string(res));
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    res = vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    if (res != VK_SUCCESS)
    {
        throw std::runtime_error("Unable to query physical Vulkan devices " 
            + std::to_string(res));
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

bool PhysicalDevice::SupportsExtensions(const std::vector<const char*> & names) const
{
    return SupportsExtensions(std::set<std::string>(names.begin(), names.end()));
}

bool PhysicalDevice::SupportsExtensions(const std::vector<std::string>  & names) const
{
    return SupportsExtensions(std::set<std::string>(names.begin(), names.end()));
}

bool PhysicalDevice::SupportsExtensions(std::set<std::string> names) const
{
    for (const auto & ext : GetAvailableExtensions()) {
        names.erase(ext);
    }

    return names.empty();
}

std::vector<std::string> PhysicalDevice::GetAvailableExtensions() const
{
    uint32_t extensionCount;

    VkResult res = vkEnumerateDeviceExtensionProperties(
        device, 
        nullptr, 
        &extensionCount, 
        nullptr
        );

    if (res != VK_SUCCESS) {
        throw std::runtime_error("Unable to query number of device extension properties, error code " 
            + std::to_string(res));
    }

    std::vector<VkExtensionProperties> extensionProperties(extensionCount);
    res = vkEnumerateDeviceExtensionProperties(
        device,
        nullptr, 
        &extensionCount, 
        extensionProperties.data()
        );

    if (res != VK_SUCCESS)
    {
        throw std::runtime_error("Unable to query device extension properties, error code " 
            + std::to_string(res));
    }

    std::vector<std::string> result;
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
        throw std::runtime_error("Unable to query surface capabilities of physical device, error code " 
            + std::to_string(res));
    }

    return capabilities;
}

std::vector<VkSurfaceFormatKHR> PhysicalDevice::GetSurfaceFormats(VkSurfaceKHR surface) const
{
    uint32_t formatCount;
    VkResult res = vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (res != VK_SUCCESS)
    {
        throw std::runtime_error("Unable to query number of swapchain format-color space pairs of physical device, error code " 
            + std::to_string(res));
    }

    std::vector<VkSurfaceFormatKHR> formats(formatCount);
    res = vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, formats.data());

    if (res != VK_SUCCESS)
    {
        throw std::runtime_error("Unable to query swapchain format-color space pairs of physical device, error code " 
            + std::to_string(res));
    }

    return formats;
}

std::vector<VkPresentModeKHR> PhysicalDevice::GetPresentModes(VkSurfaceKHR surface) const
{
    uint32_t presentModeCount;
    VkResult res = vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (res != VK_SUCCESS)
    {
        throw std::runtime_error("Unable to query number of presentation modes of physical device, error code " 
            + std::to_string(res));
    }

    std::vector<VkPresentModeKHR> modes;
    res = vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, modes.data());

    if (res != VK_SUCCESS)
    {
        throw std::runtime_error("Unable to query presentation modes of physical device, error code " 
            + std::to_string(res));
    }

    return modes;
}

std::vector<VkQueueFamilyProperties> PhysicalDevice::GetQueueFamilyProperties() const
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());


    return queueFamilies;
}

const VkPhysicalDeviceProperties & PhysicalDevice::GetProperties() const
{
    return properties;
}

