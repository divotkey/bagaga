// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
#include <stdexcept>
#include <cassert>
#include <string>

// Local includes
#include "LogicalDevice.h"
#include "CommandBuffer.h"
#include "CommandPool.h"

using namespace std;


/////////////////////////////////////////////////
/////// CommandPool
/////////////////////////////////////////////////

CommandPool::CommandPool(VkCommandPool handle, std::shared_ptr<LogicalDevice> device)
    : commandPool(handle), device(device)
{
    assert(device);
    assert(handle);
}

CommandPool::~CommandPool()
{
    if (commandPool && device) {
        vkDestroyCommandPool(*device, commandPool, nullptr);
    }
}

std::unique_ptr<CommandBuffer> CommandPool::CreateCommandBuffer(bool primary)
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = primary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer handle;
    VkResult res = vkAllocateCommandBuffers(*device, &allocInfo, &handle);

    if (res != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate command buffer, error " + to_string(res));
    }

    return unique_ptr<CommandBuffer>(new CommandBuffer(handle, shared_from_this()));
}

vector<unique_ptr<CommandBuffer>> CommandPool::CreateCommandBuffers(size_t n, bool primary)
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = primary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(n);

    vector<VkCommandBuffer> handles(n, VK_NULL_HANDLE);
    VkResult res = vkAllocateCommandBuffers(*device, &allocInfo, handles.data());

    if (res != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate command buffers, error " + to_string(res));
    }

    vector<unique_ptr<CommandBuffer>> result;
    for(auto handle : handles) {
        result.push_back(unique_ptr<CommandBuffer>(new CommandBuffer(handle, shared_from_this())));        
    }

    return result;
}

void CommandPool::Free(CommandBuffer & cmdBuf)
{
    auto handle = cmdBuf.GetHandle();
    vkFreeCommandBuffers(*device, commandPool, 1, &handle);
}

/////////////////////////////////////////////////
/////// CommandPoolBuilder
/////////////////////////////////////////////////

CommandPoolBuilder::CommandPoolBuilder()
{
    Reset();
}

CommandPoolBuilder& CommandPoolBuilder::QueueFamilyIndex(uint32_t index)
{
    queueFamilyIndex = index;
    return *this;
}

CommandPoolBuilder& CommandPoolBuilder::Flags(VkCommandPoolCreateFlags flags)
{ 
    this->flags = flags;
    return *this;
}

CommandPoolBuilder& CommandPoolBuilder::Reset()
{
    queueFamilyIndex.reset();
    flags = 0;
    return *this;
}

shared_ptr<CommandPool> CommandPoolBuilder::Build(std::shared_ptr<LogicalDevice> device)
{
    ValidateConfiguration();
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = flags;
    poolInfo.queueFamilyIndex = queueFamilyIndex.value();

    VkCommandPool handle;
    VkResult res = vkCreateCommandPool(*device, &poolInfo, nullptr, &handle);
    if (res != VK_SUCCESS) {
        throw std::runtime_error("Failed to create command pool, error" + to_string(res));
    }

    return shared_ptr<CommandPool>(new CommandPool(handle, device));
}

void CommandPoolBuilder::ValidateConfiguration()
{
    if (!queueFamilyIndex.has_value()) {
        throw logic_error("Unable to build command pool, queue family index not specified");
    }
}