// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
#include <algorithm>

// ASTU Library includes.
#include <VersionInfo.h>

// Local includes
#include "VulkanInstance.h"

using namespace std;
using namespace astu;

/////////////////////////////////////////////////
/////// VulkanInstance
/////////////////////////////////////////////////

string VulkanInstance::GetVersion() {

    uint32_t apiVersion;
    vkEnumerateInstanceVersion(&apiVersion);      

    return to_string(VK_VERSION_MAJOR(apiVersion)) 
        + "."
        + to_string(VK_VERSION_MINOR(apiVersion))
        + "."
        + to_string(VK_VERSION_PATCH(apiVersion));
}

vector<string> VulkanInstance::GetAvailableLayers()
{
    uint32_t instanceLayerCnt = 0;
    VkResult res = vkEnumerateInstanceLayerProperties(&instanceLayerCnt, NULL);

    if (res != VK_SUCCESS)
    {
        throw runtime_error("Unable to query Vulkan instance layer property count");
    }

    vector<VkLayerProperties> layerProperties(instanceLayerCnt);
    res = vkEnumerateInstanceLayerProperties(&instanceLayerCnt, layerProperties.data());
    if (res != VK_SUCCESS)
    {
        throw runtime_error("Unable to query Vulkan instance layer properties");
    }

    vector<string> result;
    for (auto & prop : layerProperties) {
        result.push_back(prop.layerName);
    }

    return result;
}

bool VulkanInstance::CheckLayers(const vector<string> & layers)
{
    auto availableLayers = GetAvailableLayers();
    for (auto & layer : layers) {
        if (find(availableLayers.begin(), availableLayers.end(), layer) == availableLayers.end()) {
            return false;
        }
    }

    return true;
}

bool VulkanInstance::CheckLayers(const vector<const char*> & layers)
{
    auto availableLayers = GetAvailableLayers();
    for (auto & layer : layers) {
        if (find(availableLayers.begin(), availableLayers.end(), layer) == availableLayers.end()) {
            return false;
        }
    }

    return true;
}

vector<string> VulkanInstance::GetAvailableExtensions()
{
    uint32_t propertyCnt = 0;
    VkResult res = vkEnumerateInstanceExtensionProperties(nullptr, &propertyCnt, NULL);

    if (res != VK_SUCCESS)
    {
        throw runtime_error("Unable to query Vulkan instance extension property count");
    }

    vector<VkExtensionProperties> extensionProperties(propertyCnt);
    res = vkEnumerateInstanceExtensionProperties(nullptr, &propertyCnt, extensionProperties.data());
    if (res != VK_SUCCESS)
    {
        throw runtime_error("Unable to query Vulkan instance extension properties");
    }

    vector<string> result;
    for (auto & prop : extensionProperties) {
        result.push_back(prop.extensionName);
    }

    return result;
}


VulkanInstance::VulkanInstance(VkInstance instance)
    : vkInstance(instance)
{
    if (!vkInstance) {
        throw invalid_argument("Vulkan instance must not be null");
    }
}

VulkanInstance::~VulkanInstance()
{
    if (vkInstance) {
        vkDestroyInstance(vkInstance, nullptr);
    }
}


/////////////////////////////////////////////////
/////// VulkanInstanceBuilder
/////////////////////////////////////////////////

VulkanInstanceBuilder::VulkanInstanceBuilder()
{
    Reset();
}

VulkanInstanceBuilder & VulkanInstanceBuilder::Reset() noexcept
{
    extensionNames.Clear();
    return *this;
}

VulkanInstanceBuilder & VulkanInstanceBuilder::AddExtension(const string & extensionName)
{

    if (extensionNames.HasName(extensionName)) {
        throw logic_error("Instance extensions has already been added (" 
            + extensionName + ")");
    }

    extensionNames.AddName(extensionName);
    return *this;
}

VulkanInstanceBuilder & VulkanInstanceBuilder::AddExtensions(const vector<string> & extensionNames)
{
    for (const auto & extensionName : extensionNames) {
        AddExtension(extensionName);
    }

    return *this;
}

VulkanInstanceBuilder & VulkanInstanceBuilder::AddExtensions(const vector<const char *> & extensionNames)
{
    for (const auto & extensionName : extensionNames) {
        AddExtension(extensionName);
    }

    return *this;
}

bool VulkanInstanceBuilder::HasExtension(const string & extensionName) const noexcept
{
    return extensionNames.HasName(extensionName);
}

VulkanInstanceBuilder & VulkanInstanceBuilder::AddLayer(const string & layerName)
{
    if (layerNames.HasName(layerName)) {
        throw logic_error("Instance layer has already been added (" 
            + layerName + ")");
    }

    layerNames.AddName(layerName);
    return *this;
}

VulkanInstanceBuilder & VulkanInstanceBuilder::AddLayers(const vector<string> & layerNames)
{
    for (const auto & layerName : layerNames) {
        AddLayer(layerName);
    }

    return *this;
}

VulkanInstanceBuilder & VulkanInstanceBuilder::AddLayers(const vector<const char *> & layerNames)
{
    for (const auto & layerName : layerNames) {
        AddLayer(layerName);
    }

    return *this;
}



bool VulkanInstanceBuilder::HasLayer(const string & extensionName) const noexcept
{
    return layerNames.HasName(extensionName);
}

unique_ptr<VulkanInstance> VulkanInstanceBuilder::Build()
{
    // Create application info (optional).
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "ASTU-Application";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "ASTU";
    appInfo.engineVersion = VK_MAKE_VERSION(GetMajorVersion(), GetMinorVersion(), GetMinorVersion());
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Create instance create info.
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;    
    createInfo.pApplicationInfo = &appInfo;    


    createInfo.enabledExtensionCount = extensionNames.NumNames();
    createInfo.ppEnabledExtensionNames = extensionNames.GetPointerArray();

    createInfo.enabledLayerCount = layerNames.NumNames();
    createInfo.ppEnabledLayerNames = layerNames.GetPointerArray();

    // Createn Vulkan instance.
    VkInstance vkInstance;
    VkResult res = vkCreateInstance(&createInfo, nullptr, &vkInstance);
    if (res != VK_SUCCESS) {
        throw runtime_error("Unable to create Vulkan instance, got error code: " 
            + to_string(res));
    }

    return  unique_ptr<VulkanInstance>(new VulkanInstance(vkInstance));
}
