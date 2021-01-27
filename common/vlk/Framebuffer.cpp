// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
#include <cassert>
#include <stdexcept>

// Local includes
#include "LogicalDevice.h"
#include "SwapChain.h"
#include "Framebuffer.h"

using namespace std;

/////////////////////////////////////////////////
/////// Framebuffer
/////////////////////////////////////////////////

Framebuffer::Framebuffer(VkFramebuffer handle, std::shared_ptr<LogicalDevice> device)
    : framebuffer(handle), device(device)
{
    assert(device);
    assert(handle);
}

Framebuffer::~Framebuffer()
{
    if (framebuffer && device) {
        vkDestroyFramebuffer(*device, framebuffer, nullptr);
    }
}


/////////////////////////////////////////////////
/////// FramebufferBuilder
/////////////////////////////////////////////////


FramebufferBuilder::FramebufferBuilder()
{
    Reset();
}

FramebufferBuilder& FramebufferBuilder::Flags(VkFramebufferCreateFlags flags)
{
    this->flags = flags;
    return *this;
}


FramebufferBuilder& FramebufferBuilder::RenderPass(VkRenderPass renderPass)
{
    assert(renderPass != VK_NULL_HANDLE);

    this->renderPass = renderPass;
    return *this;
}

FramebufferBuilder& FramebufferBuilder::AddAttachment(VkImageView attachment)
{
    attachments.push_back(attachment);
    return *this;
}

FramebufferBuilder& FramebufferBuilder::AddAttachments(const std::vector<VkImageView> & attachments)
{
    this->attachments.insert(this->attachments.end(), attachments.begin(), attachments.end());
    return *this;
}

FramebufferBuilder& FramebufferBuilder::ClearAttachments()
{
    attachments.clear();
    return *this;
}

FramebufferBuilder& FramebufferBuilder::Width(uint32_t w)
{   
    width = w;
    return *this;
}

FramebufferBuilder& FramebufferBuilder::Height(uint32_t h)
{
    height = h;
    return *this;
}

FramebufferBuilder& FramebufferBuilder::Layers(uint32_t l)
{
    layers = l;
    return *this;
}

FramebufferBuilder& FramebufferBuilder::ChooseDimension(const SwapChain & swapChain)
{
    Width(swapChain.GetImageWidth());
    Height(swapChain.GetImageHeight());
    return *this;
}

FramebufferBuilder& FramebufferBuilder::Reset()
{
    flags = 0;
    renderPass = VK_NULL_HANDLE;
    attachments.clear();
    width = 0;
    height = 0;
    layers = 1;
    return *this;
}

unique_ptr<Framebuffer> FramebufferBuilder::Build(std::shared_ptr<LogicalDevice> device)
{
    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.flags = flags;
    framebufferInfo.renderPass = renderPass;
    framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    framebufferInfo.pAttachments = attachments.empty() ? nullptr : attachments.data();
    framebufferInfo.width = width;
    framebufferInfo.height = height;
    framebufferInfo.layers = layers;


    VkFramebuffer handle;
    VkResult res = vkCreateFramebuffer(*device, &framebufferInfo, nullptr, &handle);
    if (res != VK_SUCCESS) {
        throw runtime_error("Failed to create framebuffer, error" + to_string(res));
    }

    return unique_ptr<Framebuffer>(new Framebuffer(handle, device));
}

