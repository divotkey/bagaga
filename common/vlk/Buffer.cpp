// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
#include <stdexcept>
#include <cassert>
#include <string>

// Local includes
#include "LogicalDevice.h"
#include "Buffer.h"

using namespace std;


/////////////////////////////////////////////////
/////// Buffer
/////////////////////////////////////////////////

Buffer::Buffer(VkBuffer handle, std::shared_ptr<LogicalDevice> device)
    : buffer(handle)
    , device(device)
{
    assert(buffer);
    assert(this->device);
}

Buffer::~Buffer()
{
    vkDestroyBuffer(*device, buffer, nullptr);
}

VkMemoryRequirements Buffer::GetMemoryRequirements() const
{
    VkMemoryRequirements result;
    vkGetBufferMemoryRequirements(*device, buffer, &result);

    return result;
}


/////////////////////////////////////////////////
/////// BufferBuilder
/////////////////////////////////////////////////

BufferBuilder::BufferBuilder()
{
    Reset();
}

BufferBuilder & BufferBuilder::Flags(VkBufferCreateFlags flags)
{
    this->flags = flags;
    return *this;
}

BufferBuilder & BufferBuilder::Size(VkDeviceSize size)
{
    this->size = size;
    return *this;
}

BufferBuilder & BufferBuilder::Usage(VkBufferUsageFlags usage)
{
    this->usage = usage;
    return *this;
}

BufferBuilder & BufferBuilder::SharingMode(VkSharingMode sharingMode)
{
    this->sharingMode = sharingMode;
    return *this;
}

BufferBuilder & BufferBuilder::AddQueueFamily(uint32_t index)
{
    queueFamilyIndices.push_back(index);
    return *this;
}

BufferBuilder & BufferBuilder::Reset()
{
    flags = 0;
    size = 0;
    usage = 0;
    sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    queueFamilyIndices.clear();
    return *this;
}

std::unique_ptr<Buffer> BufferBuilder::Build(std::shared_ptr<LogicalDevice> device) const
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.flags = flags;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
    bufferInfo.pQueueFamilyIndices = queueFamilyIndices.data();

    VkBuffer handle;
    VkResult res = vkCreateBuffer(*device, &bufferInfo, nullptr, &handle);
    if (res != VK_SUCCESS) {
        throw runtime_error("Failed to create Vulkan buffer object, error " + to_string(res));
    }

    return unique_ptr<Buffer>(new Buffer(handle, device));
}
