// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
#include <stdexcept>
#include <cassert>
#include <string>

// Local includes
#include "LogicalDevice.h"
#include "CommandPool.h"
#include "Buffer.h"
#include "CommandBuffer.h"

using namespace std;

CommandBuffer::CommandBuffer(VkCommandBuffer handle, std::shared_ptr<CommandPool> pool)
    : commandBuffer(handle)
    , pool(pool)
    , flags(0)
{
    assert(commandBuffer);
}

CommandBuffer::~CommandBuffer()
{
    auto pPool = pool.lock();
    if (pPool) {
        pPool->Free(*this);
    }

}

void CommandBuffer::Reset(bool releaseResources)
{
    VkResult res = vkResetCommandBuffer(commandBuffer, releaseResources ? VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT : 0);
    if (res != VK_SUCCESS) {
        throw std::runtime_error("Failed to reset command buffer, error " + to_string(res));
    }
}

void CommandBuffer::Begin()
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = flags;
    beginInfo.pInheritanceInfo = nullptr;

    VkResult res = vkBeginCommandBuffer(commandBuffer, &beginInfo);
    if (res != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer, error " + to_string(res));
    }
}

void CommandBuffer::End()
{
    VkResult res = vkEndCommandBuffer(commandBuffer);
    if (res != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer, error " + to_string(res));
    }
}

void CommandBuffer::BeginRenderPass(VkRenderPass renderPass, VkFramebuffer framebuffer, bool inlineSubpass)
{
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = framebuffer;
    renderPassInfo.renderArea = renderArea;

    // TODO make configurable.
    VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(
        commandBuffer, 
        &renderPassInfo, 
        inlineSubpass ? VK_SUBPASS_CONTENTS_INLINE : VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
}

void CommandBuffer::SetRenderArea(uint32_t w, uint32_t h, int32_t ox, int32_t oy)
{
    renderArea.offset.x = ox;
    renderArea.offset.y = oy;
    renderArea.extent.width = w;
    renderArea.extent.height = h;
}

void CommandBuffer::EndRenderPass()
{
    vkCmdEndRenderPass(commandBuffer);
}

void CommandBuffer::BindPipeline(VkPipeline pipeline, VkPipelineBindPoint bindPoint)
{
    vkCmdBindPipeline(commandBuffer, bindPoint, pipeline);
}

void CommandBuffer::BindVertexBuffer(VkBuffer buffer, size_t offset)
{
    VkDeviceSize bufferOffsets = static_cast<VkDeviceSize>(offset);
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &buffer, &bufferOffsets);
}

void CommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
    vkCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

