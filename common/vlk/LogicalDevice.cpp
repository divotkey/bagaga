// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
#include <stdexcept>
#include <algorithm>
#include <set>

// Local includes
#include "QueueIndexFinder.h"
#include "LogicalDevice.h"

using namespace std;

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

bool LogicalDevice::IsGraphicsAndPresentQueueIdentical() const
{
    return presentQueueIdx == graphicsQueueIdx;
}


/////////////////////////////////////////////////
/////// LogicalDeviceBuilder
/////////////////////////////////////////////////

const float LogicalDeviceBuilder::kQPriority = 1.0f;


LogicalDeviceBuilder::LogicalDeviceBuilder()
{
    Reset();
}

LogicalDeviceBuilder & LogicalDeviceBuilder::AddDeviceExtension(const string & name)
{
    if (HasDeviceExtension(name)) {
        throw logic_error("Device extensions already added" + name);
    }

    extensionNames.AddName(name);
    return *this;
}

LogicalDeviceBuilder & LogicalDeviceBuilder::AddDeviceExtensions(const vector<string> & extensionNames)
{
    for (const auto & extensionName : extensionNames) {
        AddDeviceExtension(extensionName);
    }

    return *this;
}

LogicalDeviceBuilder & LogicalDeviceBuilder::AddDeviceExtensions(const vector<const char *> & extensionNames)
{
    for (const auto & extensionName : extensionNames) {
        AddDeviceExtension(extensionName);
    }

    return *this;
}


bool LogicalDeviceBuilder::HasDeviceExtension(const string & name) const
{
    return extensionNames.HasName(name);
}

LogicalDeviceBuilder & LogicalDeviceBuilder::AddDeviceLayer(const string & name)
{
    if (HasDeviceLayer(name)) {
        throw logic_error("Device extensions already added" + name);
    }

    layerNames.AddName(name);
    return *this;
}

LogicalDeviceBuilder & LogicalDeviceBuilder::AddDeviceLayers(const vector<string> & layerNames)
{
    for (const auto & layerName : layerNames) {
        AddDeviceLayer(layerName);
    }

    return *this;
}

LogicalDeviceBuilder & LogicalDeviceBuilder::AddDeviceLayers(const vector<const char *> & layerNames)
{
    for (const auto & layerName : layerNames) {
        AddDeviceLayer(layerName);
    }

    return *this;
}


bool LogicalDeviceBuilder::HasDeviceLayer(const string & name) const
{
    return layerNames.HasName(name);
}

LogicalDeviceBuilder & LogicalDeviceBuilder::Reset()
{
    extensionNames.Clear();
    layerNames.Clear();
    return *this;
}

unique_ptr<LogicalDevice> LogicalDeviceBuilder::Build(VkPhysicalDevice device, VkSurfaceKHR surface) const
{

    QueueIndexFinder queueIndexFinder(device, surface);
    
    auto queueCreateInfos = BuildQueueCreateInfos(queueIndexFinder);
    auto deviceFeatures =  BuildDeviceFeatures();

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO; 
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());;
    createInfo.pEnabledFeatures = &deviceFeatures;

    // Add requested device extensions.
    createInfo.enabledExtensionCount = extensionNames.NumNames();
    createInfo.ppEnabledExtensionNames = extensionNames.GetPointerArray();

    // Device layers are deprecated, just to be on the safe side.
    createInfo.enabledLayerCount = layerNames.NumNames();
    createInfo.ppEnabledLayerNames = layerNames.GetPointerArray();

    auto result = unique_ptr<LogicalDevice>(new LogicalDevice());
    
    VkResult res = vkCreateDevice(device, &createInfo, nullptr, &result->logicalDevice);
    if (res != VK_SUCCESS) {
        throw runtime_error("Failed to create logical Vulkan device, error code" 
            + to_string(res));
    }

    // Fetch graphics queue.
    vkGetDeviceQueue(
        result->logicalDevice, 
        queueIndexFinder.GetGraphicsFamily(), 
        0, 
        &result->graphicsQueue.queue);

    // Fetch present queue.
    vkGetDeviceQueue(
        result->logicalDevice, 
        queueIndexFinder.GetPresentFamily(), 
        0, 
        &result->presentQueue.queue);

    // Fetch compute queue.
    vkGetDeviceQueue(
        result->logicalDevice, 
        queueIndexFinder.GetComputeFamily(), 
        0, 
        &result->computeQueue.queue);


    result->graphicsQueueIdx = queueIndexFinder.GetGraphicsFamily();
    result->presentQueueIdx = queueIndexFinder.GetPresentFamily();
    result->computeQueueIdx = queueIndexFinder.GetComputeFamily();

    return result;
}

vector<VkDeviceQueueCreateInfo> LogicalDeviceBuilder::BuildQueueCreateInfos(const QueueIndexFinder & qid) const
{
    vector<VkDeviceQueueCreateInfo> result;

    vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    set<uint32_t> uniqueQueueFamilies = {
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

VkPhysicalDeviceFeatures LogicalDeviceBuilder::BuildDeviceFeatures() const
{
    VkPhysicalDeviceFeatures deviceFeatures{};

    // TODO add devices features (not needed right now).

    return deviceFeatures;
}
