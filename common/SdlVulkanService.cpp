/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <VersionInfo.h>
#include "SdlVideoService.h"
#include "SdlVulkanService.h"

namespace astu {

    const std::vector<const char*> kValidationLayers = { 
        "VK_LAYER_KHRONOS_validation"
    };

    SdlVulkanService::SdlVulkanService(int priority)
        : UpdatableBaseService("SDL Vulkan", priority)
        , vkInstance(nullptr)
    {
        // Intentioniall left empty.
    }

    void SdlVulkanService::OnStartup()     
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Starting up SDL Vulkan service");

        LogVulkanInstanceVersion();

        try {
            CreateVulkanInstance();
        } catch (...) {
            throw;
        }

        // auto extensionNames = GetRequiredVulkanExtensions();
        // SDL_LogVerbose(
        //     SDL_LOG_CATEGORY_VIDEO, 
        //     "Required Vulkan Extensions: %s", 
        //     MakeCsv(extensionNames).c_str());

        auto layerNames = GetAvailableVulkanLayers();
        SDL_LogVerbose(
            SDL_LOG_CATEGORY_VIDEO, 
            "Available Vulkan Layers: %s", 
            MakeCsv(layerNames).c_str());

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
        unsigned int instancelayerCnt = 0;
        VkResult res = vkEnumerateInstanceLayerProperties(&instancelayerCnt, NULL);

        if (res != VK_SUCCESS)
        {
            SDL_LogError(SDL_LOG_CATEGORY_VIDEO, 
                "Unable to query Vulkan instance layer property count");

            throw std::runtime_error("Unable to query Vulkan instance layer property count");
        }

        std::vector<VkLayerProperties> instanceLayerNames(instancelayerCnt);
        res = vkEnumerateInstanceLayerProperties(&instancelayerCnt, instanceLayerNames.data());

        std::vector<std::string> result;
        for (auto & prop : instanceLayerNames) {
            result.push_back(prop.layerName);
        }

        return result;

    }

    void SdlVulkanService::CreateVulkanInstance()
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

        auto reqExtensions = GetRequiredVulkanExtensions();
        auto extensionNames = MakePointerList(reqExtensions);
        SDL_LogVerbose(
            SDL_LOG_CATEGORY_VIDEO, 
            "Required Vulkan Extensions: %s", 
            MakeCsv(reqExtensions).c_str());

        createInfo.enabledExtensionCount = static_cast<uint32_t>(reqExtensions.size());
        createInfo.ppEnabledExtensionNames = extensionNames.data();

        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = nullptr;

        VkResult res = vkCreateInstance(&createInfo, nullptr, &vkInstance);
        if (res != VK_SUCCESS) {

            SDL_LogError(SDL_LOG_CATEGORY_VIDEO, 
                "Unable to create Vulkan instance, got error code: %d", res);

            throw std::runtime_error("Unable to create Vulkan instance (err: " + std::to_string(res) + ").");
        }
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

        SDL_LogVerbose(
            SDL_LOG_CATEGORY_VIDEO, 
            "Vulkan instance version: %d.%d.%d", 
            VK_VERSION_MAJOR(apiVersion), VK_VERSION_MINOR(apiVersion), VK_VERSION_PATCH(apiVersion));
    }

} // end of namespace