// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// SDL includes
#include <SDL2/SDL.h>

// Local includes
#include "vlk/VulkanInstance.h"
#include "vlk/PhysicalDevice.h"
#include "SdlVulkanLogger.h"

using namespace std;

void SdlVulkanLogger::LogVulkanApiVersion()
{
    SDL_LogDebug(
        SDL_LOG_CATEGORY_VIDEO, 
        "Vulkan instance version: %s", 
        VulkanInstance::GetVersion().c_str());
}

void SdlVulkanLogger::LogVulkanExtensions()
{
    auto extensionNames = VulkanInstance::GetAvailableExtensions();
    SDL_LogVerbose(
        SDL_LOG_CATEGORY_VIDEO, 
        "Available Vulkan Extensions: %s", 
        MakeCsv(extensionNames).c_str());
}

string SdlVulkanLogger::MakeCsv(const vector<string> & names)
{
    string result;

    if (names.size() > 0) {
        result += names[0];
    }
    for (size_t i = 1; i < names.size(); ++i) {
        result += ", " + names[i];
    }

    return result;
}

void SdlVulkanLogger::LogVulkanLayers()
{
    auto layerNames = VulkanInstance::GetAvailableLayers();
    SDL_LogVerbose(
        SDL_LOG_CATEGORY_VIDEO, 
        "Available Vulkan Layers: %s", 
        MakeCsv(layerNames).c_str());
}

void SdlVulkanLogger::LogDeviceExtensions(const PhysicalDevice & device)
{
    auto extensionNames = device.GetAvailableExtensions();
    SDL_LogVerbose(
        SDL_LOG_CATEGORY_VIDEO, 
        "Available Device Extensions: %s", 
        MakeCsv(extensionNames).c_str());
}

