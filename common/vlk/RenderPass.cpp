// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// Standard C++ Library includes
#include <cassert>

// Local includes
#include "LogicalDevice.h"
#include "SwapChain.h"
#include "RenderPass.h"

using namespace std;

/////////////////////////////////////////////////
/////// RenderPass
/////////////////////////////////////////////////

RenderPass::RenderPass(VkRenderPass handle, shared_ptr<LogicalDevice> device)
    : renderPass(handle), device(device)
{
    assert(renderPass);
    assert(device);
}

RenderPass::~RenderPass()
{
    if (renderPass && device) {
        vkDestroyRenderPass(*device, renderPass, nullptr);
    }
}


/////////////////////////////////////////////////
/////// RenderPassBuilder
/////////////////////////////////////////////////

RenderPassBuilder::RenderPassBuilder()
{
    Reset();
}

RenderPassBuilder& RenderPassBuilder::Reset()
{
    colorAttachmentFormat = VK_FORMAT_UNDEFINED;
    return *this;
}

RenderPassBuilder& RenderPassBuilder::ColorAttachmentFormat(VkFormat format)
{
    colorAttachmentFormat = format;
    sampleCount = VK_SAMPLE_COUNT_1_BIT;
    return *this;
}

RenderPassBuilder& RenderPassBuilder::CooseColorAttachmentFormat(SwapChain & swapChain)
{
    return ColorAttachmentFormat(swapChain.GetImageFormat());
}


unique_ptr<RenderPass> RenderPassBuilder::Build(shared_ptr<LogicalDevice> device)
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = colorAttachmentFormat;
    colorAttachment.samples = sampleCount;

    // TODO make configurable
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;       
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    VkRenderPass renderPass;
    VkResult res = vkCreateRenderPass(*device, &renderPassInfo, nullptr, &renderPass);
    if (res != VK_SUCCESS) {
        throw runtime_error("Failed to create render pass object, error code" 
            + to_string(res));
    }

    return std::make_unique<RenderPass>(renderPass, device);    
}
