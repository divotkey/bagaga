// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
#include <cassert>
#include <string>

// Local includes
#include "PhysicalDevice.h"
#include "VertexDescription.h"

using namespace std;


/////////////////////////////////////////////////
/////// VertexBindingDescriptionBuilder
/////////////////////////////////////////////////

VertexBindingDescriptionBuilder::VertexBindingDescriptionBuilder()
{
    Reset();
}

VertexBindingDescriptionBuilder & VertexBindingDescriptionBuilder::Binding(uint32_t number)
{
    binding = number;
    return *this;
}


VertexBindingDescriptionBuilder & VertexBindingDescriptionBuilder::Stride(uint32_t bytes)
{
    stride = bytes;
    return *this;
}

VertexBindingDescriptionBuilder & VertexBindingDescriptionBuilder::InputRate(VkVertexInputRate rate)
{
    inputRate = rate;
    return *this;
}

VertexBindingDescriptionBuilder & VertexBindingDescriptionBuilder::Reset()
{
    binding = 0;
    stride = 0;
    inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return *this;
}

VkVertexInputBindingDescription VertexBindingDescriptionBuilder::Build(PhysicalDevice & device) const
{
    ValidateConfiguration(device);
    return Build();
}

VkVertexInputBindingDescription VertexBindingDescriptionBuilder::Build() const
{
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = binding;
    bindingDescription.stride = stride;
    bindingDescription.inputRate = inputRate;

    return bindingDescription;
}

void VertexBindingDescriptionBuilder::ValidateConfiguration(PhysicalDevice & device) const
{
    auto limits = device.GetLimits();

    if (binding >= limits.maxVertexInputBindings) {
        throw std::logic_error("Invalid vertex binding description, binding number exceeds limits of physical device, got " 
            + to_string(binding) + ", limit is " + to_string(limits.maxVertexInputBindings));
    }

    if (stride > limits.maxVertexInputBindingStride) {
        throw std::logic_error("Invalid vertex binding description, stride exceeds limits of physical device, got " 
            + to_string(stride) + ", limit is " + to_string(limits.maxVertexInputBindingStride));
    }
}

/////////////////////////////////////////////////
/////// VertexAttributeDescriptionBuilder
/////////////////////////////////////////////////

VertexAttributeDescriptionBuilder::VertexAttributeDescriptionBuilder()
{
    Reset();
}


VertexAttributeDescriptionBuilder & VertexAttributeDescriptionBuilder::Location(uint32_t location)
{
    this->location = location;
    return *this;
}

VertexAttributeDescriptionBuilder & VertexAttributeDescriptionBuilder::Binding(uint32_t binding)
{
    this->binding = binding;
    return *this;
}

VertexAttributeDescriptionBuilder & VertexAttributeDescriptionBuilder::Format(VkFormat format)
{
    this->format = format;
    return *this;
}

VertexAttributeDescriptionBuilder & VertexAttributeDescriptionBuilder::Offset(uint32_t offset)
{
    this->offset = offset;
    return *this;
}

VertexAttributeDescriptionBuilder & VertexAttributeDescriptionBuilder::Reset()
{
    location = 0;
    binding = 0;
    format = VK_FORMAT_UNDEFINED;
    offset = 0;
    return *this;
}

VkVertexInputAttributeDescription VertexAttributeDescriptionBuilder::Build(PhysicalDevice & device) const
{
    ValidateConfiguration(device);
    return Build();
}


VkVertexInputAttributeDescription VertexAttributeDescriptionBuilder::Build() const
{
    VkVertexInputAttributeDescription attributeDescription{};
    attributeDescription.location = location;
    attributeDescription.binding = binding;
    attributeDescription.format = format;
    attributeDescription.offset = offset;

    return attributeDescription;
}

void VertexAttributeDescriptionBuilder::ValidateConfiguration(PhysicalDevice & device) const
{
    auto limits = device.GetLimits();

    if (location > limits.maxVertexInputAttributes) {
        throw std::logic_error("Invalid vertex attribute description, location exceeds limits of physical device, got " 
            + to_string(location) + ", limit is " + to_string(limits.maxVertexInputAttributes));
    }

    if (binding >= limits.maxVertexInputBindings) {
        throw std::logic_error("Invalid vertex binding description, binding number exceeds limits of physical device, got " 
            + to_string(binding) + ", limit is " + to_string(limits.maxVertexInputBindings));
    }

    if (offset > limits.maxVertexInputAttributeOffset) {
        throw std::logic_error("Invalid vertex attribute description, offset exceeds limits of physical device, got " 
            + to_string(offset) + ", limit is " + to_string(limits.maxVertexInputAttributeOffset));
    }

}
