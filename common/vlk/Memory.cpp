// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
#include <stdexcept>
#include <cassert>
#include <string>

// Local includes
#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include "Memory.h"

using namespace std;


/////////////////////////////////////////////////
/////// Memory
/////////////////////////////////////////////////

Memory::Memory(VkDeviceMemory handle, std::shared_ptr<LogicalDevice> device)
    : memory(handle)
    , device(device)
    , allocationSize(0)
    , mapped(false)
    , data(nullptr)
{
    assert(memory);
    assert(this->device);
}

Memory::~Memory()
{
    vkFreeMemory(*device, memory, nullptr);
}

size_t Memory::GetSize() const
{
    return static_cast<size_t>(allocationSize);
}

void Memory::WriteData(const std::vector<unsigned char> data, size_t offset)
{
    auto data = Map(data.size(), offset);
}

unsigned char* Memory::Map(size_t size, size_t offset)
{
    if (mapped) {
        throw logic_error("Unable to map memory object, because object is already mapped");
    }

    if (size - offset > allocationSize) {
        throws logic_error("Unable to map memory object, size/offset combination exceeds allocated memory");
    }

    void *data;
    VkResult res = vkMapMemory(
        *device, 
        memory, 
        static_cast<VkDeviceSize>(offset), 
        static_cast<VkDeviceSize>(size), 
        0, 
        &data);

    if (res != VK_SUCCESS) {
        throw runtime_error("Failed to map memory object into application address space, error " + to_string(res));
    }
    this->data = reinterpret_cast<unsigned char*>(data);

    mapped = true;
    return this->data;
}


void Memory::Unmap()
{
    if (!mapped) {
        throw std::logic_error("Unable to unmap memory object, because object is not mapped");
    }
    vkUnmapMemory(*device, memory);
    mapped = false;
    data = nullptr;
}


/////////////////////////////////////////////////
/////// MemoryBuilder
/////////////////////////////////////////////////

MemoryBuilder::MemoryBuilder()
{
    Reset();
}

MemoryBuilder & MemoryBuilder::AllocationSize(VkDeviceSize size)
{
    allocationSize = size;
    return *this;
}

MemoryBuilder & MemoryBuilder::MemoryTypeIndex(uint32_t index)
{
    memoryTypeIndex = index;
    return *this;
}

MemoryBuilder & MemoryBuilder::ChooseMemoryTypeIndex(const PhysicalDevice & device, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    return MemoryTypeIndex(FindMemoryType(device.GetMemoryProperties(), typeFilter, properties));
    return *this;
}

uint32_t MemoryBuilder::FindMemoryType(
    const VkPhysicalDeviceMemoryProperties &memProperties, uint32_t typeFilter, 
    const VkMemoryPropertyFlags & properties) const
{
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if (typeFilter & (1 << i) 
            && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    throw runtime_error("Failed to find suitable memory type");
}



MemoryBuilder & MemoryBuilder::Reset()
{
    memoryTypeIndex = 0;
    allocationSize = 0;
    return *this;
}

std::unique_ptr<Memory> MemoryBuilder::Build(std::shared_ptr<LogicalDevice> device) const
{
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = allocationSize;
    allocInfo.memoryTypeIndex = memoryTypeIndex;

    VkDeviceMemory handle;
    VkResult res = vkAllocateMemory(*device, &allocInfo, nullptr, &handle);

    if (res != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate device memory, error " + to_string(res));
    }

    auto result = unique_ptr<Memory>(new Memory(handle, device));
    result->allocationSize = allocationSize;
    return result;
}
