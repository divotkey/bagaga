// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// Standard C++ Library includes
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits> 

// Local includes
#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include "SwapChain.h"

using namespace std;


/////////////////////////////////////////////////
/////// SwapChain
/////////////////////////////////////////////////

SwapChain::SwapChain(VkSwapchainKHR handle, std::shared_ptr<LogicalDevice> device, VkFormat format, VkExtent2D extent)
    : swapChain(handle)
    , device(device)
    , imageFormat(format)
    , extent(extent)
{
    assert(device);

    uint32_t imageCount;
    VkResult res = vkGetSwapchainImagesKHR(*device, swapChain, &imageCount, nullptr);
    if (res != VK_SUCCESS) {
        throw runtime_error("Failed to query image count of swap chain, error code" 
            + to_string(res));
    }

    images.resize(imageCount);

    res = vkGetSwapchainImagesKHR(*device, swapChain, &imageCount, images.data());

    if (res != VK_SUCCESS) {
        throw runtime_error("Failed to query images of swap chain, error code" 
            + to_string(res));
    }

    // Create image views.
    for (const auto & image : images) {
        imageViews.push_back( CreateImageView(*device, image) );
    }

    // for (size_t i = 0; i < images.size(); ++i) {
    //     VkImageViewCreateInfo createInfo{};
    //     createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    //     createInfo.image = images[i];     
    //     createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    //     createInfo.format = GetImageFormat();
    //     createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    //     createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    //     createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    //     createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;  

    //     createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    //     createInfo.subresourceRange.baseMipLevel = 0;
    //     createInfo.subresourceRange.levelCount = 1;
    //     createInfo.subresourceRange.baseArrayLayer = 0;
    //     createInfo.subresourceRange.layerCount = 1;              

    //     res = vkCreateImageView(device, &createInfo, nullptr, &imageViews[i])        
    // }
}

SwapChain::~SwapChain()
{
    std::shared_ptr<LogicalDevice> pDevice = device.lock();
    if (pDevice) {
        vkDestroySwapchainKHR(*pDevice, swapChain, nullptr);
    }
}

VkImageView SwapChain::CreateImageView(const LogicalDevice& device, const VkImage& image) const
{
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = image;     
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = GetImageFormat();
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;  

    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;   

    VkImageView view;
    VkResult res = vkCreateImageView(device, &createInfo, nullptr, &view);

    if (res != VK_SUCCESS) {
        throw runtime_error("Failed to create image view, error code" 
            + to_string(res));
    }

    return view;                  
}


/////////////////////////////////////////////////
/////// SwapChainBuilder
/////////////////////////////////////////////////

SwapChainBuilder::SwapChainBuilder()
{
    Reset();
}

SwapChainBuilder & SwapChainBuilder::ImageFormat(VkFormat format)
{
    imageFormat = format;
    return *this;
}

SwapChainBuilder & SwapChainBuilder::ColorSpace(VkColorSpaceKHR cs)
{
    colorSpace = cs;
    return *this;
}

SwapChainBuilder & SwapChainBuilder::ChooseSurfaceFormat(const PhysicalDevice & device, VkSurfaceKHR surface)
{
    auto formats = device.GetSurfaceFormats(surface);
    if (formats.empty()) {
        throw runtime_error(string("Unable to choose image format for device ") 
            + device.GetName() + ", no image formats available");
    }

    for (auto const & availableFormat : formats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB 
            && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return SurfaceFormat(availableFormat.format, availableFormat.colorSpace);
        }
    }

    return SurfaceFormat(formats[0].format, formats[0].colorSpace);
}

SwapChainBuilder & SwapChainBuilder::ChooseSurfaceFormat(const VkPhysicalDevice device, VkSurfaceKHR surface)
{
    return ChooseSurfaceFormat(PhysicalDevice(device), surface);
}

SwapChainBuilder & SwapChainBuilder::PresentMode(VkPresentModeKHR mode)
{
    presentMode = mode;
    return *this;
}

SwapChainBuilder & SwapChainBuilder::ImageExtent(uint32_t width, uint32_t height)
{
    extent.width = width;
    extent.height = height;
    return *this;
}

SwapChainBuilder & SwapChainBuilder::CooseImageExtend(const VkPhysicalDevice device, VkSurfaceKHR surface, int fbw, int fbh)
{
    return CooseImageExtend(PhysicalDevice(device), surface, fbw, fbh);
}

SwapChainBuilder & SwapChainBuilder::CooseImageExtend(const PhysicalDevice & device, VkSurfaceKHR surface, int fbw, int fbh)
{
    auto capabilities = device.GetSurfaceCapabilities(surface);
    if (capabilities.currentExtent.width != (numeric_limits<uint32_t>::max)()) {
        return ImageExtent(capabilities.currentExtent.width, capabilities.currentExtent.height);
    } else {
        return ImageExtent(
            clamp(
                static_cast<uint32_t>(fbw), 
                capabilities.minImageExtent.width, 
                capabilities.maxImageExtent.width), 
            clamp(
                static_cast<uint32_t>(fbh), 
                capabilities.minImageExtent.height, 
                capabilities.maxImageExtent.height)
            );        
    }
    return *this;
}

SwapChainBuilder & SwapChainBuilder::ImageCount(uint32_t cnt)
{
    imageCount = cnt;
    return *this;
}

SwapChainBuilder & SwapChainBuilder::CooseImageCount(const PhysicalDevice & device, VkSurfaceKHR surface)
{
    auto capabilities = device.GetSurfaceCapabilities(surface);

    uint32_t cnt = capabilities.minImageCount + 1;
    if(capabilities.maxImageCount > 0 && capabilities.maxImageCount > cnt) {
        cnt = capabilities.maxImageCount;
    }
    return ImageCount(cnt);
}

SwapChainBuilder & SwapChainBuilder::Transform(VkSurfaceTransformFlagBitsKHR tx)
{
    transform = tx;
    return *this;
}

SwapChainBuilder & SwapChainBuilder::CooseTransform(const PhysicalDevice & device, VkSurfaceKHR surface)
{
    return Transform(device.GetSurfaceCapabilities(surface).currentTransform);
}

SwapChainBuilder & SwapChainBuilder::ChooseConfiguration(const PhysicalDevice & device, VkSurfaceKHR surface, int fbw, int fbh)
{
    ChooseSurfaceFormat(device, surface);
    CooseImageExtend(device, surface, fbw, fbh);
    CooseImageCount(device, surface);
    CooseTransform(device, surface);
    return *this;
}

SwapChainBuilder & SwapChainBuilder::Reset()
{
    imageFormat = VK_FORMAT_B8G8R8A8_SRGB;
    presentMode = VK_PRESENT_MODE_FIFO_KHR;
    oldSwapChain = VK_NULL_HANDLE;
    extent.width = (numeric_limits<uint32_t>::max)();
    extent.height = (numeric_limits<uint32_t>::max)();
    imageCount = 0;
    transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;

    return *this;
}

unique_ptr<SwapChain> SwapChainBuilder::Build(const std::shared_ptr<LogicalDevice> device, VkSurfaceKHR surface)
{
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = imageFormat;
    createInfo.imageColorSpace = colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    vector<uint32_t> queueFamilyIndices;
    if (device->IsGraphicsAndPresentQueueIdentical()) {
        assert(device->GetGraphicsQueueIndex() == device->GetPresentQueueIndex());

        queueFamilyIndices = {device->GetGraphicsQueueIndex()};
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 1;                           // Optional
        createInfo.pQueueFamilyIndices = queueFamilyIndices.data();     // Optional
    } else {
        assert(device->GetGraphicsQueueIndex() != device->GetPresentQueueIndex());

        queueFamilyIndices = {device->GetGraphicsQueueIndex(), device->GetPresentQueueIndex()};
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
    }
    
    createInfo.preTransform = transform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = oldSwapChain;
    createInfo.presentMode= presentMode;

    VkSwapchainKHR handle;
    VkResult res = vkCreateSwapchainKHR(*device, &createInfo, nullptr, &handle);
    if (res != VK_SUCCESS) {
        throw runtime_error("Failed to create swap chain, error code" 
            + to_string(res));
    }

    return unique_ptr<SwapChain>(new SwapChain(handle, device, imageFormat, extent));
}
