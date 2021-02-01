// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// C++ Standard Library includes
#include <cstdint>
#include <memory>

// Vulkan includes
#include <vulkan/vulkan.h>

// Forward declaration
class PhysicalDevice;

/**
 * Utility class to build vertex input binding description structures.
 */
class VertexBindingDescriptionBuilder {
public:

    /**
     * Constructor.
     */
    VertexBindingDescriptionBuilder();


    /***
     * Specifies the binding number for the vertex description to use.
     * 
     * @param number    the binding number
     * @return reference to this builder used for method chaining
     */
    VertexBindingDescriptionBuilder & Binding(uint32_t number);

    /**
     * Specifies the distance in bytes between two consecutive elements within the buffer.
     *
     * @param bytes    the number of bytes between two elements
     * @return reference to this builder used for method chaining
     */
    VertexBindingDescriptionBuilder & Stride(uint32_t bytes);

    /**
     * Specifies whether vertex attribute addressing is a function of the vertex index or of the instance index.
     *
     * @param rate  the input rate
     * @return reference to this builder used for method chaining
     */
    VertexBindingDescriptionBuilder & InputRate(VkVertexInputRate rate);

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    VertexBindingDescriptionBuilder & Reset();

    /**
     * Builds the vertex input binding description according to the current configuration.
     * This version takes a physical Vulkan device to validate the current configuration
     * 
     * @param device    the logical device used to validate the configuration.
     * @return the newly created vertex binding description structure
     * @throws std::logic_error in case the configuration is invalid
     */
    VkVertexInputBindingDescription Build(PhysicalDevice & device) const;

    /**
     * Builds the vertex input binding description according to the current configuration.
     * 
     * @return the newly created vertex binding description structure
     */
    VkVertexInputBindingDescription Build() const;


private:

    /** The binding number for the vertex description. */
    uint32_t binding;

    /** Is the distance in bytes between two consecutive elements within the buffer. */
    uint32_t stride;

    /** Specifies whether vertex attribute addressing is a function of the vertex index or of the instance index. */
    VkVertexInputRate inputRate;

    /**
     * Validates the configuration.
     * 
     * @throws std::logic_error in case the configuration is invalid
     */
    void ValidateConfiguration(PhysicalDevice & device) const;
};

/**
 * Utility class to build vertex input attribute description structures.
 */
class VertexAttributeDescriptionBuilder {
public:

    /**
     * Constructor.
     */
    VertexAttributeDescriptionBuilder();

    /**
     * Specifies the shader binding location number for the attribute.
     * 
     * @param location  the shader binding location
     * @return reference to this builder used for method chaining
     */
    VertexAttributeDescriptionBuilder & Location(uint32_t location);

    /**
     * Specifies the binding number which the attribute takes its data from.
     * 
     * @param binding  the binding number
     * @return reference to this builder used for method chaining
     */
    VertexAttributeDescriptionBuilder & Binding(uint32_t binding);

    /**
     * Specifies the size and type of the vertex attribute data.
     * 
     * @param format  the attribute format
     * @return reference to this builder used for method chaining
     */
    VertexAttributeDescriptionBuilder & Format(VkFormat format);

    /**
     * Specifies the byte offset of this attribute relative to the start of an element in the vertex input binding.
     * 
     * @param offset    the byte offset
     * @return reference to this builder used for method chaining
     */
    VertexAttributeDescriptionBuilder & Offset(uint32_t offset);

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    VertexAttributeDescriptionBuilder & Reset();

    /**
     * Builds the vertex input attribute description according to the current configuration.
     * This version takes a physical Vulkan device to validate the current configuration
     * 
     * @param device    the logical device used to validate the configuration.
     * @return the newly created vertex attribute description structure
     * @throws std::logic_error in case the configuration is invalid
     */
    VkVertexInputAttributeDescription Build(PhysicalDevice & device) const;

    /**
     * Builds the vertex input attribute description according to the current configuration.
     * 
     * @return the newly created vertex attribute description structure
     */
    VkVertexInputAttributeDescription Build() const;


private:

    /** The shader binding location number for the attribute. */
    uint32_t location;

    /** The binding number which the attribute takes its data from. */
    uint32_t binding;

    /** The size and type of the vertex attribute data. */
    VkFormat format;    

    /** The byte offset of this attribute relative to the start of an element in the vertex input binding. */
    uint32_t offset;

    /**
     * Validates the configuration.
     * 
     * @throws std::logic_error in case the configuration is invalid
     */
    void ValidateConfiguration(PhysicalDevice & device) const;
};