// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
#include <algorithm>
#include <cassert>
#include <vector>
#include <set>
#include <map>

// SDL includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

// Astu includes
#include <VersionInfo.h>
#include <SdlVideoService.h>
#include <Vector2.h>
#include <Vector3.h>
#include <ITimeService.h>

// Local includes
#include "vlk/SdlVulkanLogger.h"
#include "vlk/VulkanInstance.h"
#include "vlk/QueueIndexFinder.h"
#include "vlk/LogicalDevice.h"
#include "vlk/SwapChain.h"
#include "vlk/Framebuffer.h"
#include "vlk/RenderPass.h"
#include "vlk/GraphicsPipeline.h"
#include "vlk/CommandPool.h"
#include "vlk/CommandBuffer.h"
#include "vlk/Semaphore.h"
#include "vlk/VertexInputInfo.h"
#include "vlk/VertexDescription.h"
#include "vlk/Memory.h"
#include "vlk/Buffer.h"
#include "vlk/InputAssembly.h"
#include "vlk/ViewportState.h"
#include "vlk/Rasterizer.h"
#include "vlk/Multisampling.h"
#include "vlk/ColorBlendState.h"
#include "vlk/PipelineLayout.h"
#include "vlk/ShaderStage.h"
#include "vlk/shd/frag.h"
#include "vlk/shd/vert2.h"
#include "SdlVulkanService.h"

using namespace astu;


struct Vertex {
    Vector2<float> pos;
    Vector3<float> col;
};

std::vector<Vertex> vertices = {
    {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};

const std::vector<const char*> SdlVulkanService::kValidationLayers = { 
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> SdlVulkanService::kDeviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

SdlVulkanService::SdlVulkanService(bool debug, int priority)
    : UpdatableBaseService("SDL Vulkan", priority)
    , enableValidationLayers(debug)
    , debugMessenger(VK_NULL_HANDLE)
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
        CreateFramebuffers();
        CreateVertexBuffer();
        CreateCommandBuffers();
        CreateSemaphores();
    } catch (...) {
        Cleanup();
        throw;
    }

    absoluteTime = 0;
}

void SdlVulkanService::OnShutdown() 
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Shutting down SDL Vulkan service");
    Cleanup();
}

void SdlVulkanService::OnUpdate() 
{
        // std::optional<uint32_t> AcquireNextImage(VkSemaphore semaphore = VK_NULL_HANDLE, uint64_t timeout = UINT64_MAX);

    auto imageIndex = swapChain->AcquireNextImage(*imageAvailableSemaphore);
    if(!imageIndex.has_value()) {
        SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, 
            "Aquire next image from swap chain failed");

        return;
    }

    absoluteTime += ServiceManager::GetInstance().GetService<ITimeService>().GetElapsedTime();
    vertices[0].pos.x = static_cast<float>(sin(absoluteTime) / 2);
    vertices[1].pos.y = 0.5f + static_cast<float>(sin(absoluteTime * 0.7) * 0.35);
    vertices[2].pos.y = 0.5f + static_cast<float>(sin(absoluteTime * 1.8) * -0.45);
    // cout << vertices[0].col.x << endl;
    
    vertexBuffer->GetBoundedMemory()->WriteData(
        reinterpret_cast<const unsigned char*>(vertices.data()), 
        sizeof(vertices[0]) * vertices.size());

    logicalDevice->GetGraphicsQueue().ClearWaitSemaphores();
    logicalDevice->GetGraphicsQueue().AddWaitSemaphore(imageAvailableSemaphore, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
    logicalDevice->GetGraphicsQueue().ClearSignalSemaphores();
    logicalDevice->GetGraphicsQueue().AddSignalSemaphore(renderFinishedSemaphore);
    logicalDevice->GetGraphicsQueue().SubmitCommandBuffer(*commandBuffers.at(imageIndex.value()));


    logicalDevice->GetPresentQueue().WaitIdle();
    logicalDevice->GetPresentQueue().ClearWaitSemaphores();
    logicalDevice->GetPresentQueue().AddWaitSemaphore(renderFinishedSemaphore);
    logicalDevice->GetPresentQueue().Present(*swapChain, imageIndex.value());
}

void SdlVulkanService::Cleanup()
{
    imageAvailableSemaphore = nullptr;
    renderFinishedSemaphore = nullptr;
    commandBuffers.clear();
    commandPool = nullptr;
    framebuffers.clear();
    renderPass = nullptr;
    swapChain = nullptr;
    logicalDevice = nullptr;

    if (surface) {
        vkDestroySurfaceKHR(*instance, surface, nullptr);
        surface = VK_NULL_HANDLE;
    }

    if (debugMessenger) {
        DestroyDebugUtilsMessengerEXT(debugMessenger);
        debugMessenger = VK_NULL_HANDLE;
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
        .AddAttachment(
            AttachmentBuilder()
            .Format(swapChain->GetImageFormat())
            .Samples(VK_SAMPLE_COUNT_1_BIT)
            .LoadOp(VK_ATTACHMENT_LOAD_OP_CLEAR)
            .StoreOp(VK_ATTACHMENT_STORE_OP_STORE)
            .InitialLayout(VK_IMAGE_LAYOUT_UNDEFINED)
            .FinalLayout(VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
            .Build())
        .AddSubpass(
            SubpassBuilder()
            .PipelineBindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS)
            .AddColorAttachment(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
            .Build())
        .AddSubpassDependency(
            SubpassDependencyBuilder()
            .SrcSubpass(VK_SUBPASS_EXTERNAL)
            .SrcStageMask(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT)
            .SrcAccessMask(0)
            .DstSubpass(0)
            .DstStageMask(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT)
            .DstAccessMask(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT)
            .Build())  
        .Build(logicalDevice);

    SDL_LogVerbose(SDL_LOG_CATEGORY_VIDEO, "Successfully created render pass object");
}

void SdlVulkanService::CreateGraphicsPipeline()
{
    GraphicsPipelineBuilder builder;

    builder.VertexInputState(
        VertexInputInfoBuilder()
        .AddVertexBindingDescription(
            VertexBindingDescriptionBuilder()
                .Binding(0)
                .Stride(sizeof(Vertex))
                .InputRate(VK_VERTEX_INPUT_RATE_VERTEX)
                .Build())
        .AddVertexAttributeDescription(
            VertexAttributeDescriptionBuilder()
                .Location(0)
                .Binding(0)
                .Format(VK_FORMAT_R32G32_SFLOAT)
                .Offset(offsetof(Vertex, pos))
                .Build())
        .AddVertexAttributeDescription(
            VertexAttributeDescriptionBuilder()
                .Location(1)
                .Binding(0)
                .Format(VK_FORMAT_R32G32B32_SFLOAT)
                .Offset(offsetof(Vertex, col))
                .Build())
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

void SdlVulkanService::CreateFramebuffers()
{
    FramebufferBuilder fbBuilder;
    fbBuilder.RenderPass(*renderPass);
    fbBuilder.ChooseDimension(*swapChain).Layers(1);
    for (auto imageView : swapChain->GetImageViews()) {
        fbBuilder.ClearAttachments();
        fbBuilder.AddAttachment(imageView);
        framebuffers.push_back(fbBuilder.Build(logicalDevice));
    }
    SDL_LogVerbose(SDL_LOG_CATEGORY_VIDEO, "Successfully created framebuffer objects");
}

void SdlVulkanService::CreateCommandBuffers()
{
    commandPool = CommandPoolBuilder()
        .QueueFamilyIndex(logicalDevice->GetGraphicsQueueIndex())
        .Build(logicalDevice);

    SDL_LogVerbose(SDL_LOG_CATEGORY_VIDEO, "Successfully created command pool");

    commandBuffers = commandPool->CreateCommandBuffers(framebuffers.size());
    SDL_LogVerbose(SDL_LOG_CATEGORY_VIDEO, "Successfully created command buffers");

    for (size_t i = 0; i < framebuffers.size(); ++i) {
        auto & cmdBuf = *commandBuffers[i];
        cmdBuf.Begin();
        cmdBuf.SetRenderArea(swapChain->GetImageWidth(), swapChain->GetImageHeight(), 0, 0);
        cmdBuf.BeginRenderPass(*renderPass, *framebuffers[i]);
        cmdBuf.BindPipeline(*graphicsPipeline, VK_PIPELINE_BIND_POINT_GRAPHICS);
        cmdBuf.BindVertexBuffer(*vertexBuffer);
        cmdBuf.Draw(vertices.size(), 1, 0, 0);
        cmdBuf.EndRenderPass();
        cmdBuf.End();
    }

    SDL_LogVerbose(SDL_LOG_CATEGORY_VIDEO, "Successfully recorded command buffers");
}

void SdlVulkanService::CreateSemaphores()
{
    imageAvailableSemaphore = Semaphore::Create(logicalDevice);
    renderFinishedSemaphore = Semaphore::Create(logicalDevice);
    SDL_LogVerbose(SDL_LOG_CATEGORY_VIDEO, "Successfully created semaphore objects");
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

void SdlVulkanService::CreateVertexBuffer()
{
    vertexBuffer = BufferBuilder()
        .Size(sizeof(vertices[0]) * vertices.size())
        .Usage(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)
        .SharingMode(VK_SHARING_MODE_EXCLUSIVE)
        // not required because exclusive mode
        .AddQueueFamily(logicalDevice->GetGraphicsQueueIndex())
        .Build(logicalDevice);

    const auto memRequirements = vertexBuffer->GetMemoryRequirements();

    std::shared_ptr<Memory> memory = MemoryBuilder()
        .AllocationSize(memRequirements.size)
        .ChooseMemoryTypeIndex(
            *physicalDevice, 
            memRequirements.memoryTypeBits, 
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT 
            | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
        .Build(logicalDevice);

    vertexBuffer->BindMemory(memory);

    memory->WriteData(
        reinterpret_cast<const unsigned char*>(vertices.data()), 
        sizeof(vertices[0]) * vertices.size());
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
