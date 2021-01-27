// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
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
/////// SubpassDependencyBuilder
/////////////////////////////////////////////////

SubpassDependencyBuilder::SubpassDependencyBuilder()
{
    Reset();
}

SubpassDependencyBuilder & SubpassDependencyBuilder::SrcSubpass(uint32_t index)
{
    srcSubpass = index;
    return *this;
}

SubpassDependencyBuilder & SubpassDependencyBuilder::DstSubpass(uint32_t index)
{
    dstSubpass = index;
    return *this;
}

SubpassDependencyBuilder & SubpassDependencyBuilder::SrcStageMask(VkPipelineStageFlags stageMask)
{
    srcStageMask = stageMask;
    return *this;
}

SubpassDependencyBuilder & SubpassDependencyBuilder::DstStageMask(VkPipelineStageFlags stageMask)
{
    dstStageMask = stageMask;
    return *this;
}

SubpassDependencyBuilder & SubpassDependencyBuilder::SrcAccessMask(VkAccessFlags accessMask)
{
    srcAccessMask = accessMask;
    return *this;
}

SubpassDependencyBuilder & SubpassDependencyBuilder::DstAccessMask(VkAccessFlags accessMask)
{
    dstAccessMask = accessMask;
    return *this;
}

SubpassDependencyBuilder & SubpassDependencyBuilder::DependencyFlags(VkDependencyFlags flags)
{
    dependencyFlags = flags;
    return *this;
}

SubpassDependencyBuilder & SubpassDependencyBuilder::Reset()
{
    srcSubpass = 0;
    dstSubpass = 0;
    srcStageMask = 0;
    dstStageMask = 0;
    srcAccessMask = 0;
    dstAccessMask = 0;   
    dependencyFlags = 0; 
    return *this;
}

VkSubpassDependency SubpassDependencyBuilder::Build()
{
    VkSubpassDependency dependency{};
    dependency.srcSubpass = srcSubpass;
    dependency.dstSubpass = dstSubpass;
    dependency.srcStageMask = srcStageMask;
    dependency.dstStageMask = dstStageMask;
    dependency.srcAccessMask = srcAccessMask;
    dependency.dstAccessMask = dstAccessMask;
    dependency.dependencyFlags = dependencyFlags;

    return dependency;
}


/////////////////////////////////////////////////
/////// AttachmentBuilder
/////////////////////////////////////////////////

AttachmentBuilder::AttachmentBuilder()
{
    Reset();
}

AttachmentBuilder& AttachmentBuilder::Format(VkFormat format)
{
    this->format = format;
    return *this;
}

AttachmentBuilder& AttachmentBuilder::Samples(VkSampleCountFlagBits samples)
{
    this->samples = samples;
    return *this;
}

AttachmentBuilder& AttachmentBuilder::LoadOp(VkAttachmentLoadOp op)
{
    loadOp = op;
    return *this;
}

AttachmentBuilder& AttachmentBuilder::StoreOp(VkAttachmentStoreOp op)
{
    storeOp = op;
    return *this;
}

AttachmentBuilder& AttachmentBuilder::StencilLoadOp(VkAttachmentLoadOp op)
{
    stencilLoadOp = op;
    return *this;
}

AttachmentBuilder& AttachmentBuilder::StencilStoreOp(VkAttachmentStoreOp op)
{
    stencilStoreOp = op;
    return *this;
}

AttachmentBuilder& AttachmentBuilder::InitialLayout(VkImageLayout layout)
{
    initialLayout = layout;
    return *this;
}

AttachmentBuilder& AttachmentBuilder::FinalLayout(VkImageLayout layout)
{
    finalLayout = layout;
    return *this;
}

AttachmentBuilder& AttachmentBuilder::Reset()
{
    flags = 0;
    format = VK_FORMAT_UNDEFINED;
    samples = VK_SAMPLE_COUNT_1_BIT;
    loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    finalLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    return *this;
}

VkAttachmentDescription AttachmentBuilder::Build() const
{
    VkAttachmentDescription attachment{};
    attachment.flags = flags;
    attachment.format = format;
    attachment.samples = samples;
    attachment.loadOp = loadOp;
    attachment.storeOp = storeOp;
    attachment.stencilLoadOp = stencilLoadOp;
    attachment.stencilStoreOp = stencilStoreOp;
    attachment.initialLayout = initialLayout;
    attachment.finalLayout = finalLayout;

    return attachment;
}


/////////////////////////////////////////////////
/////// SubpassBuilder
/////////////////////////////////////////////////

 
SubpassBuilder::SubpassBuilder()
{
    Reset();
}

SubpassBuilder& SubpassBuilder::Flags(VkSubpassDescriptionFlags flags)
{
    this->flags = flags;
    return *this;
}

SubpassBuilder& SubpassBuilder::PipelineBindPoint(VkPipelineBindPoint bindPoint)
{
    pipelineBindPoint = bindPoint;
    return *this;
}

SubpassBuilder& SubpassBuilder::AddInputAttachment(uint32_t index, VkImageLayout layout)
{
    VkAttachmentReference ref{};
    ref.attachment = index;
    ref.layout = layout;
    inputAttachments.push_back(ref);
    return *this;
}

SubpassBuilder& SubpassBuilder::AddColorAttachment(uint32_t index, VkImageLayout layout)
{
    VkAttachmentReference ref{};
    ref.attachment = index;
    ref.layout = layout;
    colorAttachments.push_back(ref);
    return *this;
}

SubpassBuilder& SubpassBuilder::AddResolveAttachment(uint32_t index, VkImageLayout layout)
{
    VkAttachmentReference ref{};
    ref.attachment = index;
    ref.layout = layout;
    resolveAttachments.push_back(ref);
    return *this;
}

SubpassBuilder& SubpassBuilder::AddDepthStencilAttachment(uint32_t index, VkImageLayout layout)
{
    VkAttachmentReference ref{};
    ref.attachment = index;
    ref.layout = layout;
    depthStencilAttachments.push_back(ref);
    return *this;
}

SubpassBuilder& SubpassBuilder::AddPreserveAttachment(uint32_t index)
{
    preserveAttachments.push_back(index);
    return *this;
}

SubpassBuilder& SubpassBuilder::Reset()
{
    flags = 0;
    pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    inputAttachments.clear();
    colorAttachments.clear();
    resolveAttachments.clear();
    depthStencilAttachments.clear();
    preserveAttachments.clear();

    return *this;
}

Subpass SubpassBuilder::Build() const
{
    Subpass result;

    VkSubpassDescription subpass{};
    result.subpass.flags = flags;
    result.subpass.pipelineBindPoint = pipelineBindPoint;

    result.subpass.inputAttachmentCount = static_cast<uint32_t>(inputAttachments.size());
    result.inputAttachments = inputAttachments;
    result.subpass.pInputAttachments = result.inputAttachments.data();

    result.subpass.colorAttachmentCount = static_cast<uint32_t>(colorAttachments.size());
    result.colorAttachments = colorAttachments;
    result.subpass.pColorAttachments = result.colorAttachments.data();

    result.resolveAttachments = resolveAttachments;
    result.subpass.pResolveAttachments = result.resolveAttachments.data();

    result.depthStencilAttachments = depthStencilAttachments;
    result.subpass.pDepthStencilAttachment = result.depthStencilAttachments.data();

    result.subpass.preserveAttachmentCount = static_cast<uint32_t>(preserveAttachments.size());
    result.preserveAttachments = preserveAttachments;
    result.subpass.pPreserveAttachments = result.preserveAttachments.data();

    return result;
}


/////////////////////////////////////////////////
/////// RenderPassBuilder
/////////////////////////////////////////////////

RenderPassBuilder::RenderPassBuilder()
{
    Reset();
}

RenderPassBuilder&  RenderPassBuilder::AddAttachment(const VkAttachmentDescription & attachment)
{
    attachments.push_back(attachment);    
    return *this;
}

RenderPassBuilder& RenderPassBuilder::AddSubpass(const Subpass & subpass)
{
    subpasses.push_back(subpass);
    return *this;
}

RenderPassBuilder& RenderPassBuilder::AddSubpassDependency(const VkSubpassDependency & dependency)
{
    subpassDependencies.push_back(dependency);
    return *this;
}

RenderPassBuilder& RenderPassBuilder::Reset()
{
    attachments.clear();
    subpasses.clear();
    subpassDependencies.clear();
    return *this;
}

unique_ptr<RenderPass> RenderPassBuilder::Build(shared_ptr<LogicalDevice> device) const
{

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();

    std::vector<VkSubpassDescription> subpassDescriptions;
    for (const auto & subpass : subpasses) {
        subpassDescriptions.push_back(subpass.GetDescription());
    }
    renderPassInfo.subpassCount = static_cast<uint32_t>(subpasses.size());;
    renderPassInfo.pSubpasses = subpassDescriptions.data();
    renderPassInfo.dependencyCount = static_cast<uint32_t>(subpassDependencies.size());
    renderPassInfo.pDependencies = subpassDependencies.data();

    VkRenderPass renderPass;
    VkResult res = vkCreateRenderPass(*device, &renderPassInfo, nullptr, &renderPass);
    if (res != VK_SUCCESS) {
        throw runtime_error("Failed to create render pass object, error code" 
            + to_string(res));
    }

    return std::unique_ptr<RenderPass>(new RenderPass(renderPass, device));    
}
