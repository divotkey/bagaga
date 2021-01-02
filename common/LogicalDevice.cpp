// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#include <stdexcept>
#include <algorithm>
#include <set>

#include "QueueIndexFinder.h"
#include "LogicalDevice.h"


/////////////////////////////////////////////////
/////// LogicalDevice
////////////////////////////////////////////////

LogicalDevice::LogicalDevice()
    : logicalDevice(nullptr)
    , graphicsQueue(nullptr)
    , presentQueue(nullptr)
    , computeQueue(nullptr)
{
    // Intentionally left empty.
}

LogicalDevice::~LogicalDevice()
{
    vkDestroyDevice(logicalDevice, nullptr);
}


/////////////////////////////////////////////////
/////// LogicalDeviceBuilder
/////////////////////////////////////////////////

const float LogicalDeviceBuilder::kQPriority = 1.0f;


LogicalDeviceBuilder::LogicalDeviceBuilder()
{
    Reset();
}

LogicalDeviceBuilder & LogicalDeviceBuilder::AddDeviceExtension(const std::string & name)
{
    if (HasDeviceExtension(name)) {
        throw std::logic_error("Device extensions already added" + name);
    }

    extensionNames.push_back(name);
    return *this;
}

bool LogicalDeviceBuilder::HasDeviceExtension(const std::string & name) const
{
    return std::find(extensionNames.begin(), extensionNames.end(), name) != extensionNames.end();
}

LogicalDeviceBuilder & LogicalDeviceBuilder::AddDeviceLayer(const std::string & name)
{
    if (HasDeviceLayer(name)) {
        throw std::logic_error("Device extensions already added" + name);
    }

    layerNames.push_back(name);
    return *this;
}

bool LogicalDeviceBuilder::HasDeviceLayer(const std::string & name) const
{
    return std::find(layerNames.begin(), layerNames.end(), name) != layerNames.end();
}

LogicalDeviceBuilder & LogicalDeviceBuilder::Reset()
{
    extensionNames.clear();
    layerNames.clear();
    return *this;
}

std::unique_ptr<LogicalDevice> LogicalDeviceBuilder::Build(VkPhysicalDevice device, VkSurfaceKHR surface)
{

    QueueIndexFinder queueIndexFinder(device, surface);
    
    auto queueCreateInfos = BuildQueueCreateInfos(queueIndexFinder);
    auto deviceFeatures =  BuildDeviceFeatures();
    auto deviceExtensions = GetPointers(extensionNames);

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO; 
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());;
    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    auto result = std::unique_ptr<LogicalDevice>(new LogicalDevice());
    
    VkResult res = vkCreateDevice(device, &createInfo, nullptr, &result->logicalDevice);
    if (res != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical Vulkan device, error code" 
            + std::to_string(res));
    }

    // Fetch graphics queue.
    vkGetDeviceQueue(
        result->logicalDevice, 
        queueIndexFinder.GetGraphicsFamily(), 
        0, 
        &result->graphicsQueue);

    // Fetch present queue.
    vkGetDeviceQueue(
        result->logicalDevice, 
        queueIndexFinder.GetPresentFamily(), 
        0, 
        &result->presentQueue);

    // Fetch compute queue.
    vkGetDeviceQueue(
        result->logicalDevice, 
        queueIndexFinder.GetComputeFamily(), 
        0, 
        &result->computeQueue);

    return result;
}

std::vector<VkDeviceQueueCreateInfo> LogicalDeviceBuilder::BuildQueueCreateInfos(const QueueIndexFinder & qid)
{
    std::vector<VkDeviceQueueCreateInfo> result;

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {
        qid.GetGraphicsFamily(), 
        qid.GetComputeFamily(), 
        qid.GetPresentFamily(), 
        };

    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &kQPriority;

        result.push_back(queueCreateInfo);
    }

    return result;
}

VkPhysicalDeviceFeatures LogicalDeviceBuilder::BuildDeviceFeatures()
{
    VkPhysicalDeviceFeatures deviceFeatures{};

    // TODO add devices features (not needed right now).

    return deviceFeatures;
}


std::vector<const char*> LogicalDeviceBuilder::GetPointers(const std::vector<std::string> & names) const
{
    std::vector<const char*> result;

    for (const auto & name : names) {
        result.push_back(name.data());
    }
    return result;
}
