// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// Standard C++ Library includes
#include <algorithm>
#include <cassert>
#include <set>
#include <map>

// SDL includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

// Astu includes
#include <VersionInfo.h>
#include <SdlVideoService.h>
#include <SdlVulkanService.h>

// Local includes
#include "vlk/SdlVulkanLogger.h"
#include "vlk/VulkanInstance.h"
#include "vlk/QueueIndexFinder.h"
#include "vlk/LogicalDevice.h"

using namespace astu;

const std::vector<const char*> SdlVulkanService::kValidationLayers = { 
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> SdlVulkanService::kDeviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

SdlVulkanService::SdlVulkanService(bool debug, int priority)
    : UpdatableBaseService("SDL Vulkan", priority)
    , enableValidationLayers(debug)
    , debugMessenger(nullptr)
{
    // Intentioniall left empty.
}

SdlVulkanService::~SdlVulkanService()
{
    // Intentionally left empty.
}

void SdlVulkanService::OnStartup()     
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Starting up SDL Vulkan service");

    SdlVulkanLogger::LogVulkanApiVersion();
    // SdlVulkanLogger::LogVulkanExtensions();
    // SdlVulkanLogger::LogVulkanLayers();

    try {
        CreateVulkanInstance();
        if (enableValidationLayers) {
            InitializeDebugMessenger();
        }

        CreateSurface();
        SelectPhysicalDevice();
        // SdlVulkanLogger::LogDeviceExtensions(*physicalDevice);
        CreateLogicalDevice();
    } catch (...) {
        Cleanup();
        throw;
    }
}

void SdlVulkanService::OnShutdown() 
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Shutting down SDL Vulkan service");
    Cleanup();
}

void SdlVulkanService::OnUpdate() 
{

}

void SdlVulkanService::Cleanup()
{
    if (logicalDevice) {
        logicalDevice = nullptr;
    }

    if (surface) {
        vkDestroySurfaceKHR(*instance, surface, nullptr);
        surface = nullptr;
    }

    if (debugMessenger) {
        DestroyDebugUtilsMessengerEXT(debugMessenger);
        debugMessenger = nullptr;
    }

    physicalDevice = nullptr;
}

std::vector<std::string> SdlVulkanService::GetRequiredVulkanExtensions() const
{
    auto & videoSrv = ServiceManager::GetInstance().GetService<SdlVideoService>();
    unsigned int extCnt;

    if (!SDL_Vulkan_GetInstanceExtensions(videoSrv.GetSdlWindow(), &extCnt, nullptr)) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, 
            "Unable to query the number of Vulkan instance extensions: %s"
            , SDL_GetError());

        throw std::runtime_error(SDL_GetError());
    }

    std::vector<const char*> extNames(extCnt);

    if (!SDL_Vulkan_GetInstanceExtensions(videoSrv.GetSdlWindow(), &extCnt, extNames.data())) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, 
            "Unable to query the number of Vulkan instance extensions: %s"
            , SDL_GetError());

        throw std::runtime_error(SDL_GetError());
    }

    std::vector<std::string> result;
    for (auto & name : extNames) {
        result.push_back(name);
    }

    return result;
}

void SdlVulkanService::CreateVulkanInstance()
{
    VulkanInstanceBuilder builder;

    if (enableValidationLayers) {
        if (!VulkanInstance::CheckLayers(kValidationLayers)) {
            std::string txt = "Vulkan validation layers not available";
            SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "%s", txt.c_str());
            throw std::runtime_error(txt);
        }
        builder.AddExtension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        builder.AddLayers(kValidationLayers);
    }

    builder.AddExtensions(GetRequiredVulkanExtensions());

    instance = builder.Build();
}

void SdlVulkanService::InitializeDebugMessenger()
{
    // TODO implement
}

void SdlVulkanService::SelectPhysicalDevice()
{
    assert(instance);

    auto devices = PhysicalDevice::GetVulkanDevices(*instance);

    std::multimap<int, PhysicalDevice> candidates;
    for (const auto & device : devices) {
        int score = RatePhysicalDevice(device);
        candidates.insert(std::make_pair(score, device));
    }

    if (candidates.empty()) {
        throw std::runtime_error("No physical Vulkan device found");
    }

    physicalDevice = std::make_unique<PhysicalDevice>(candidates.rbegin()->second);

    SDL_LogInfo(
        SDL_LOG_CATEGORY_VIDEO, 
        "Using Vulkan device %s", 
        physicalDevice->GetName());
}

void SdlVulkanService::CreateLogicalDevice()
{
    assert(physicalDevice);
    assert(surface);

    LogicalDeviceBuilder builder;

    builder.AddDeviceExtensions(kDeviceExtensions);
    builder.AddDeviceLayers(kValidationLayers);
    logicalDevice = builder.Build(*physicalDevice, surface);
}

void SdlVulkanService::CreateSurface() 
{
    assert(instance);

    auto sdlWindow = ServiceManager::GetInstance().GetService<SdlVideoService>().GetSdlWindow();
    if (!SDL_Vulkan_CreateSurface(sdlWindow, *instance, &surface)) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, 
            "Unable to query the number of Vulkan instance extensions: %s"
            , SDL_GetError());
        throw std::runtime_error(SDL_GetError());
    }
}

int SdlVulkanService::RatePhysicalDevice(const PhysicalDevice & device) const
{

    if (!device.SupportsExtensions(kDeviceExtensions)) {
        return 0;
    }

    if (!QueueIndexFinder(device, surface).HasAllFamilies())
    {
        return 0;
    }

    int score = 0;
    score += device.IsDiscreteGPU() ? 1000 : 0;
    score += device.GetMaxImageDimension2D();

    SDL_LogVerbose(SDL_LOG_CATEGORY_VIDEO, 
        "Found Vulkan device %s (score %d)", device.GetName(), score);

    return score;   
}

/////////////////////////////////////////////////
/////// Proxy functions
/////////////////////////////////////////////////

void SdlVulkanService::DestroyDebugUtilsMessengerEXT(VkDebugUtilsMessengerEXT dbgMsngr) 
{
    assert(instance);

    auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>
        (vkGetInstanceProcAddr(*instance, "vkDestroyDebugUtilsMessengerEXT"));

    if (!func) {
        throw std::runtime_error("Unable to load vkDestroyDebugUtilsMessengerEXT function");
    }

    func(*instance, dbgMsngr, nullptr);
}

VkResult SdlVulkanService::CreateDebugUtilsMessengerEXT(
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
    VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    assert(instance);

    auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>
        (vkGetInstanceProcAddr(*instance, "vkCreateDebugUtilsMessengerEXT"));
    
    if (!func) {
        throw std::runtime_error("Unable to load vkCreateDebugUtilsMessengerEXT function");
    }

    return func(*instance, pCreateInfo, nullptr, pDebugMessenger);
}
