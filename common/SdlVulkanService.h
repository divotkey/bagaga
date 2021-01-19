// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// Standard C++ Library includes
#include <memory>
#include <string>
#include <vector>

// Vulkan SDK includes
#include <vulkan/vulkan.h>

// AST Utilities Library includes
#include <UpdateService.h>

// Local includes
#include "vlk/PhysicalDevice.h"

// Forward declaration.
class VulkanInstance;
class LogicalDevice;
class SwapChain;
class RenderPass;

class SdlVulkanService : public astu::UpdatableBaseService {
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

    /**
     * Virtual destructor.
     * 
     * This destructor is required because a unique pointer to
     * class LogicalDevice is used with only a forward declaration.
     */
    virtual ~SdlVulkanService();

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
    std::unique_ptr<VulkanInstance> instance;

    /** Represents the logical Vulkan device. */
    std::shared_ptr<LogicalDevice> logicalDevice;

    /** Used to receive debug messages from Vulkan validation layer. */
    VkDebugUtilsMessengerEXT debugMessenger;

    /** The physical device to be used. */
    std::unique_ptr<PhysicalDevice> physicalDevice;

    /** The swap chain used to present the rendered images. */
    std::unique_ptr<SwapChain> swapChain;

    /** The render pass object. */
    std::unique_ptr<RenderPass> renderPass;

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
     * Creates the Vulkan instance.
     */
    void CreateVulkanInstance();

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
     * Creates the swap chain.
     */
    void CreateSwapChain();

    /**
     * Creates the render pass object.
     */
    void CreateRenderPass();    

    /**
     * Creates the window surface.
     */
    void CreateSurface();

    /**
     * Rates a physical device.
     * 
     * Better suited devices get a higher score.
     * 
     * @param device    the device to rate
     * @return the score the device has achieve
     */
    int RatePhysicalDevice(const PhysicalDevice & device) const;

    /**
     * Retrieves the required Vulkan extentions from SDL.
     * 
     * @return the required Vulkan extensions
     */
    std::vector<std::string> GetRequiredVulkanExtensions() const;

    /////// Proxy functions (simplified) ///////
    void DestroyDebugUtilsMessengerEXT(VkDebugUtilsMessengerEXT dbgMsngr);

    VkResult CreateDebugUtilsMessengerEXT(
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        VkDebugUtilsMessengerEXT* pDebugMessenger);
};
