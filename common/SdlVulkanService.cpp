// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
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

// Local includes
#include "vlk/SdlVulkanLogger.h"
#include "vlk/VulkanInstance.h"
#include "vlk/QueueIndexFinder.h"
#include "vlk/LogicalDevice.h"
#include "vlk/SwapChain.h"
#include "vlk/RenderPass.h"
#include "vlk/GraphicsPipeline.h"
#include "vlk/VertexInputInfo.h"
#include "vlk/InputAssembly.h"
#include "vlk/ViewportState.h"
#include "vlk/Rasterizer.h"
#include "vlk/Multisampling.h"
#include "vlk/ColorBlendState.h"
#include "vlk/PipelineLayout.h"
#include "vlk/ShaderStage.h"
#include "vlk/shd/frag.h"
#include "vlk/shd/vert.h"
#include "SdlVulkanService.h"

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
        CreateSwapChain();    
        CreateRenderPass();
        CreateGraphicsPipeline();
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
    if (renderPass) {
        renderPass = nullptr;
    }

    if (swapChain) {
        swapChain = nullptr;
    }

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
        if (score > 0) {
            candidates.insert(std::make_pair(score, device));
        }
    }

    if (candidates.empty()) {
        throw std::runtime_error("No suitable physical Vulkan device found");
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

void SdlVulkanService::CreateSwapChain()
{
    assert(physicalDevice);
    assert(logicalDevice);
    assert(surface);

    auto sdlWindow = ServiceManager::GetInstance().GetService<SdlVideoService>().GetSdlWindow();
    int width;
    int height;
    SDL_Vulkan_GetDrawableSize(sdlWindow, &width, &height);

    SwapChainBuilder builder;
    builder.ChooseConfiguration(*physicalDevice, surface, width, height);

    swapChain = builder.Build(logicalDevice, surface);
    SDL_LogVerbose(SDL_LOG_CATEGORY_VIDEO, "Successfully created swap chain");
}

void SdlVulkanService::CreateRenderPass()
{
    assert(logicalDevice);
    assert(swapChain);

    renderPass = RenderPassBuilder()
        .ChooseColorAttachmentFormat(*swapChain)        
        .Build(logicalDevice);

    SDL_LogVerbose(SDL_LOG_CATEGORY_VIDEO, "Successfully created render pass object");
}

void SdlVulkanService::CreateGraphicsPipeline()
{
    GraphicsPipelineBuilder builder;

    builder.VertexInputInfo(
        VertexInputInfoBuilder()
        .Build());

    builder.InputAssembly(
        InputAssemblyBuilder()
        .Topology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
        .PrimitiveRestart(false).Build()
        );

    builder.ViewportState(
        ViewportStateBuilder()
        .AddViewport(ViewportBuilder().ChoosePositionAndSize(*swapChain).Build())
        .Build());

    builder.Rasterizer(
        RasterizerBuilder()
        .EnableDepthClamp(false)
        .EnableRasterizerDiscard(false)
        .PolygonMode(VK_POLYGON_MODE_FILL)
        .LineWidth(1.0f)
        .CullMode(VK_CULL_MODE_BACK_BIT)
        .FrontFace(VK_FRONT_FACE_CLOCKWISE)
        .EnableDepthBias(false)
        .DepthBiasConstantFactor(0.0f)
        .DepthBiasClamp(0.0f)
        .DepthBiasSlopeFactor(0.0f)
        .Build());

    builder.Multisampling(
        MultisamplingBuilder()
        .EnableSampleShading(false)
        .RasterizationSamples(VK_SAMPLE_COUNT_1_BIT)
        .MinSampleShading(1.0f)
        .EnableAlphaToCoverage(false)
        .EnableAlphaToOne(false)
        .Build());

    builder.ColorBlending(
        ColorBlendStateBuilder()
        .EnableLogicOp(false)
        .LogicOp(VK_LOGIC_OP_COPY)
        .AddAttachment(
            BlendAttachmentStateBuilder()
            .EnableBlend(false)
            .SrcColorBlendFactor(VK_BLEND_FACTOR_ONE)
            .DstColorBlendFactor(VK_BLEND_FACTOR_ZERO)
            .ColorBlendOp(VK_BLEND_OP_ADD)
            .SrcAlphaBlendFactor(VK_BLEND_FACTOR_ONE)
            .DstAlphaBlendFactor(VK_BLEND_FACTOR_ZERO)
            .AlphaBlendOp(VK_BLEND_OP_ADD)
            .Build())
        .Build());

    builder.Layout(
        PipelineLayoutBuilder()
        .Build(logicalDevice));


    builder.RenderPass(*renderPass, 0);

            // ShaderModuleBuilder()
            // .Code(kVertexShader)
            // .Build(logicalDevice))


    builder.AddShaderStage(
        ShaderStageBuilder()
        .Stage(VK_SHADER_STAGE_VERTEX_BIT)
        .EntryPoint("main")
        .Module(
            ShaderModuleBuilder()
            .Code(kVertexShader)
            .Build(logicalDevice))
        .Build());

    builder.AddShaderStage(
        ShaderStageBuilder()
        .Stage(VK_SHADER_STAGE_FRAGMENT_BIT)
        .EntryPoint("main")
        .Module(
            ShaderModuleBuilder()
            .Code(kFragmentShader)
            .Build(logicalDevice))
        .Build());

    graphicsPipeline = builder.Build(logicalDevice);
    SDL_LogVerbose(SDL_LOG_CATEGORY_VIDEO, "Successfully created graphics pipeline object");
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

    if (device.GetSurfaceFormats(surface).empty()) {
        return 0;
    }

    if (device.GetPresentModes(surface).empty()) {
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
