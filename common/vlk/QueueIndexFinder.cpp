// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#include <stdexcept>
#include <vector>

#include "QueueIndexFinder.h"

QueueIndexFinder::QueueIndexFinder()
{
    // Intentionally left empty.
}

QueueIndexFinder::QueueIndexFinder(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    Search(device, surface);
}

void QueueIndexFinder::Search(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    for (unsigned int i = 0; i < queueFamilies.size(); ++i) {
        const auto & queueFamily = queueFamilies[i];
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsFamily = i;
        }

        if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
            computeFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if (presentSupport) {
            presentFamily = i;
        }

        if (HasAllFamilies())
            break;
    }
}

bool QueueIndexFinder::HasGraphicsFamily() const
{
    return graphicsFamily.has_value();
}

uint32_t QueueIndexFinder::GetGraphicsFamily() const
{
    if (!graphicsFamily.has_value()) {
        throw std::logic_error("No graphics queue family found");
    }

    return graphicsFamily.value();
}

bool QueueIndexFinder::HasPresentFamily() const
{
    return presentFamily.has_value();
}

uint32_t QueueIndexFinder::GetPresentFamily() const
{
    if (!presentFamily.has_value()) {
        throw std::logic_error("No present queue family found");
    }

    return presentFamily.value();
}

bool QueueIndexFinder::HasComputeFamily() const
{
    return computeFamily.has_value();
}

uint32_t QueueIndexFinder::GetComputeFamily() const
{
    if (!computeFamily.has_value()) {
        throw std::logic_error("No compute queue family found");
    }

    return computeFamily.value();
}

bool QueueIndexFinder::HasAllFamilies() const
{
    return HasGraphicsFamily() && HasPresentFamily() && HasComputeFamily();
}