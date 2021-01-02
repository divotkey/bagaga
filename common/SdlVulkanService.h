/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <string>
#include <optional>
#include <vector>
#include <vulkan/vulkan.h>
#include "UpdateService.h"

namespace astu {

    class SdlVulkanService : public UpdatableBaseService {
    public:

        /**
         * Constructor.
         * 
         * Using this service in debug mode will activate Vulkan validation layers.
         * 
         * @param debug     whether to run this service in debug mode
         * @param priority  the priority used to update this service
         */
        SdlVulkanService(bool debug = false, int priority = 0);

    protected:

        // Inherited via UpdatableBaseService
        virtual void OnStartup() override;
        virtual void OnShutdown() override;
        virtual void OnUpdate() override;

    private:
        /** The name of used validation layers in debug mode. */
        static const std::vector<const char*> kValidationLayers;        

        /** The name of used device extensions. */
        static const std::vector<const char*> kDeviceExtensions;        

        /** Determines whether to use Vulkan validation layers. */
        bool enableValidationLayers;

        /** The Vulkan instance. */
        VkInstance vkInstance;

        /** Used to receive debug messages from Vulkan validation layer. */
        VkDebugUtilsMessengerEXT debugMessenger;

        /** The physical device to be used. */
        VkPhysicalDevice physicalDevice;

        /** The logical device. */
        VkDevice logicalDevice;

        /** The vulkan graphics queue of the logical device. */
        VkQueue graphicsQueue;

        /** The vulkan presentation queue of the logical device. */
        VkQueue presentQueue;

        /** The vulkan compute queue of the logical device. */
        VkQueue computeQueue;

        /** Used to present rendered images. */
        VkSurfaceKHR surface;

        /**
         * Debug message callback function.
         * 
         * This static method is used to receive messages from the Vulkan validation layer.
         * 
         * @param messageSeverity
         * @param messageType
         * @param pCallbackData
         * @param pUserData
         */
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);

        /**
         * Frees all aquired resources.
         */
        void Cleanup();

        /**
         * Emits a log messages describing the Vulkan version.
         */
        void LogVulkanInstanceVersion() const;

        /**
         * Emits a log messages describing available Vulkan extensions.
         */
        void LogVulkanExtensions() const;


        /**
         * Emits a log messages describing available Vulkan device extensions.
         */
        void LogDeviceExtensions(VkPhysicalDevice device) const;

        /**
         * Emits a log messages describing available Vulkan layers.
         */
        void LogVulkanLayers() const;

        /**
         * Creates the Vulkan instance.
         */
        void CreateVulkanInstance();

        /**
         * Checks whether validation layers are available.
         * 
         * @return `true` if validation layers are available
         */
        bool CheckValidationLayers();

        /**
         * Checks whether a physical device supports the required extensions.
         * 
         * @param device    the physical device to test
         */
        bool CheckDeviceExtensions(VkPhysicalDevice device) const;

        /**
         * Setup the debug message call back.
         */
        void InitializeDebugMessenger();

        /**
         * Selects the physical Vulkan device to use.
         */
        void SelectPhysicalDevice();

        /**
         * Creates the logical Vulkan device.
         */
        void CreateLogicalDevice();

        /**
         * Creates the window surface.
         */
        void CreateSurface();

        int RatePhysicalDevice(VkPhysicalDevice device) const;
        void LogPhysicalDevice() const;
        

        std::vector<std::string> GetRequiredVulkanExtensions() const;
        std::vector<std::string> GetAvailableVulkanExtensions() const;
        std::vector<std::string> GetAvailableVulkanLayers() const;
        std::vector<std::string> GetAvailableDeviceExtensions(VkPhysicalDevice device) const;
        std::string MakeCsv(const std::vector<std::string> & names) const;
        std::vector<const char*> MakePointerList(const std::vector<std::string> & names) const;

        // Proxy functions (simplified)
        void DestroyDebugUtilsMessengerEXT(VkDebugUtilsMessengerEXT dbgMsngr);
        
        VkResult CreateDebugUtilsMessengerEXT(
            const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
            VkDebugUtilsMessengerEXT* pDebugMessenger);
    };


} // end of namespace