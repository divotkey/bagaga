/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <cassert>
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <VersionInfo.h>
#include "SdlVideoService.h"
#include "SdlVulkanService.h"

namespace astu {

    const std::vector<const char*> SdlVulkanService::kValidationLayers = { 
        "VK_LAYER_KHRONOS_validation"
    };

    SdlVulkanService::SdlVulkanService(bool debug, int priority)
        : UpdatableBaseService("SDL Vulkan", priority)
        , enableValidationLayers(debug)
        , vkInstance(nullptr)
        , debugMessenger(nullptr)
        , physicalDevice(nullptr)
    {
        // Intentioniall left empty.
    }

    void SdlVulkanService::OnStartup()     
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Starting up SDL Vulkan service");

        LogVulkanInstanceVersion();
        LogVulkanExtensions();
        LogVulkanLayers();

        try {
            CreateVulkanInstance();
            if (enableValidationLayers) {
                InitializeDebugMessenger();
            }

            SelectPhysicalDevice();

        } catch (...) {
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
        if (debugMessenger) {
            DestroyDebugUtilsMessengerEXT(debugMessenger);
            debugMessenger = nullptr;
        }

        if (vkInstance) {
            vkDestroyInstance(vkInstance, nullptr);
            vkInstance = nullptr;
        }
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

    std::vector<std::string> SdlVulkanService::GetAvailableVulkanLayers() const
    {
        uint32_t instanceLayerCnt = 0;
        VkResult res = vkEnumerateInstanceLayerProperties(&instanceLayerCnt, NULL);

        if (res != VK_SUCCESS)
        {
            SDL_LogError(SDL_LOG_CATEGORY_VIDEO, 
                "Unable to query Vulkan instance layer property count");

            throw std::runtime_error("Unable to query Vulkan instance layer property count");
        }

        std::vector<VkLayerProperties> layerProperties(instanceLayerCnt);
        res = vkEnumerateInstanceLayerProperties(&instanceLayerCnt, layerProperties.data());
        if (res != VK_SUCCESS)
        {
            SDL_LogError(SDL_LOG_CATEGORY_VIDEO, 
                "Unable to query Vulkan instance layer properties");

            throw std::runtime_error("Unable to query Vulkan instance layer properties");
        }

        std::vector<std::string> result;
        for (auto & prop : layerProperties) {
            result.push_back(prop.layerName);
        }

        return result;
    }

    std::vector<std::string> SdlVulkanService::GetAvailableVulkanExtensions() const
    {
        uint32_t propertyCnt = 0;
        VkResult res = vkEnumerateInstanceExtensionProperties(nullptr, &propertyCnt, NULL);

        if (res != VK_SUCCESS)
        {
            SDL_LogError(SDL_LOG_CATEGORY_VIDEO, 
                "Unable to query Vulkan instance extension property count");

            throw std::runtime_error("Unable to query Vulkan instance extension property count");
        }

        std::vector<VkExtensionProperties> extensionProperties(propertyCnt);
        res = vkEnumerateInstanceExtensionProperties(nullptr, &propertyCnt, extensionProperties.data());
        if (res != VK_SUCCESS)
        {
            SDL_LogError(SDL_LOG_CATEGORY_VIDEO, 
                "Unable to query Vulkan instance extension properties");

            throw std::runtime_error("Unable to query Vulkan instance extension properties");
        }

        std::vector<std::string> result;
        for (auto & prop : extensionProperties) {
            result.push_back(prop.extensionName);
        }

        return result;
    }

    void SdlVulkanService::CreateVulkanInstance()
    {
        assert(vkInstance == nullptr);
        
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

        auto reqExtensions = GetRequiredVulkanExtensions();
        if (enableValidationLayers) {
            reqExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        auto extensionNames = MakePointerList(reqExtensions);
        SDL_LogVerbose(
            SDL_LOG_CATEGORY_VIDEO, 
            "Required Vulkan Extensions: %s", 
            MakeCsv(reqExtensions).c_str());

        createInfo.enabledExtensionCount = static_cast<uint32_t>(reqExtensions.size());
        createInfo.ppEnabledExtensionNames = extensionNames.data();

        if (enableValidationLayers) {
            if (!CheckValidationLayers()) {
                SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Vulkan validation layers not available");
                throw std::runtime_error("Vulkan validation layers not available");
            }

            SDL_LogDebug(SDL_LOG_CATEGORY_VIDEO, "Enabling Vuklan validation layers");
            createInfo.enabledLayerCount = kValidationLayers.size();
            createInfo.ppEnabledLayerNames = kValidationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
            createInfo.ppEnabledLayerNames = nullptr;
        }   

        VkResult res = vkCreateInstance(&createInfo, nullptr, &vkInstance);
        if (res != VK_SUCCESS) {

            SDL_LogError(SDL_LOG_CATEGORY_VIDEO, 
                "Unable to create Vulkan instance, got error code: %d", res);

            throw std::runtime_error("Unable to create Vulkan instance (err: " + std::to_string(res) + ").");
        }
    }

    void SdlVulkanService::InitializeDebugMessenger()
    {
        // TODO implement
    }

    void SdlVulkanService::SelectPhysicalDevice()
    {
        assert(vkInstance);

        uint32_t deviceCount = 0;
        VkResult res = vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);

        if (res != VK_SUCCESS)
        {
            SDL_LogError(SDL_LOG_CATEGORY_VIDEO, 
                "Unable to query number of physical Vulkan devices");

            throw std::runtime_error("Unable to query number of physical Vulkan devices");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        res = vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices.data());
        if (res != VK_SUCCESS)
        {
            SDL_LogError(SDL_LOG_CATEGORY_VIDEO, 
                "Unable to query physical Vulkan devices");

            throw std::runtime_error("Unable to query physical Vulkan devices");
        }

        for (const auto & device : devices) {
            LogPhysicalDevice(device);
        }

    }

    int SdlVulkanService::RatePhysicalDevice(VkPhysicalDevice device) const
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);     

        int score = 0;


        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            score += 1000;
        }

        score += deviceProperties.limits.maxImageDimension2D;

        // TODO add more features.

        return score;   
    }

    void SdlVulkanService::LogPhysicalDevice(VkPhysicalDevice device) const
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);     
        LogPhysicalDevice(deviceProperties);
    }

    void SdlVulkanService::LogPhysicalDevice(const VkPhysicalDeviceProperties & deviceProperties) const
    {
        SDL_LogVerbose(
            SDL_LOG_CATEGORY_VIDEO, 
            "Vulkan device: %s", 
            deviceProperties.deviceName);
    }


    bool SdlVulkanService::CheckValidationLayers()
    {
        auto presentLayers = GetAvailableVulkanLayers();
        for (const auto & layer : kValidationLayers) {
            if (std::find(presentLayers.begin(), presentLayers.end(), layer) == presentLayers.end()) {
                return false;
            }
        }

        return true;
    }

    std::string SdlVulkanService::MakeCsv(const std::vector<std::string> & items) const
    {
        std::string result;

        if (items.size() > 0) {
            result += items[0];
        }
        for (size_t i = 1; i < items.size(); ++i) {
            result += ", " + items[i];
        }

        return result;
    }

    std::vector<const char*> SdlVulkanService::MakePointerList(const std::vector<std::string> & names) const
    {
        std::vector<const char*> result;
        for (auto & name : names) {
            result.push_back(name.c_str());
        }

        return result;
    }

    void SdlVulkanService::LogVulkanInstanceVersion() const
    {
        uint32_t apiVersion;
        vkEnumerateInstanceVersion(&apiVersion);      

        VK_VERSION_MAJOR(apiVersion);  

        SDL_LogDebug(
            SDL_LOG_CATEGORY_VIDEO, 
            "Vulkan instance version: %d.%d.%d", 
            VK_VERSION_MAJOR(apiVersion), VK_VERSION_MINOR(apiVersion), VK_VERSION_PATCH(apiVersion));
    }

    void SdlVulkanService::LogVulkanExtensions() const
    {
        auto extensionNames = GetAvailableVulkanExtensions();
        SDL_LogVerbose(
            SDL_LOG_CATEGORY_VIDEO, 
            "Available Vulkan Extensions: %s", 
            MakeCsv(extensionNames).c_str());
    }

    void SdlVulkanService::LogVulkanLayers() const
    {
        auto layerNames = GetAvailableVulkanLayers();
        SDL_LogVerbose(
            SDL_LOG_CATEGORY_VIDEO, 
            "Available Vulkan Layers: %s", 
            MakeCsv(layerNames).c_str());
    }

    /////////////////////////////////////////////////
    /////// Proxy functions
    /////////////////////////////////////////////////

    void SdlVulkanService::DestroyDebugUtilsMessengerEXT(VkDebugUtilsMessengerEXT dbgMsngr) 
    {
        assert(vkInstance);

        auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>
            (vkGetInstanceProcAddr(vkInstance, "vkDestroyDebugUtilsMessengerEXT"));

        if (!func) {
            throw std::runtime_error("Unable to load vkDestroyDebugUtilsMessengerEXT function");
        }

        func(vkInstance, dbgMsngr, nullptr);
    }

    VkResult SdlVulkanService::CreateDebugUtilsMessengerEXT(
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
        VkDebugUtilsMessengerEXT* pDebugMessenger)
    {
        assert(vkInstance);

        auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>
            (vkGetInstanceProcAddr(vkInstance, "vkCreateDebugUtilsMessengerEXT"));
        
        if (!func) {
            throw std::runtime_error("Unable to load vkCreateDebugUtilsMessengerEXT function");
        }

        return func(vkInstance, pCreateInfo, nullptr, pDebugMessenger);
    }



} // end of namespace