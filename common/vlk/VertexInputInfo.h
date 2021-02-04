// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// C++ Standard Library includes
#include <vector>

// Vulkan includes
#include <vulkan/vulkan.h>

/**
 * Wraps Vulkan vertex input state descriptions structure and contains related data.
 */
class VertexInputInfo {
public:

    /**
     * Constructor.
     * 
     * @param vertexInputInfo       the vertex input info description
     * @param bindingDescriptions   the vertex input binding descriptions
     * @param attributeDescriptions the vertex input binding descriptions
     */
    VertexInputInfo(
        const VkPipelineVertexInputStateCreateInfo & vertexInputInfo,
        const std::vector<VkVertexInputBindingDescription> & bindingDescriptions, 
        const std::vector<VkVertexInputAttributeDescription> & attributeDescriptions);

    /**
     * Copy constructor.
     * 
     * @param o the other instance
     */
    VertexInputInfo(const VertexInputInfo & o);


    /**
     * Copy assignment operator.
     * 
     * @param rhs   the right hand side
     * @return reference to this instance
     */
    VertexInputInfo & operator= (const VertexInputInfo & rhs);

    /**
     * Returns the Vulkan vertex input state structure.
     * 
     * @return the vertex input state structure
     */
    const VkPipelineVertexInputStateCreateInfo & GetInfo() const {
        return vertexInputInfo;
    }

    /**
     * Implicit conversion to Vulkan vertex input state structure
     */
    operator VkPipelineVertexInputStateCreateInfo() const { 
        return GetInfo();
    }

private:
    /** The vertex input state description. */
    VkPipelineVertexInputStateCreateInfo vertexInputInfo;

    /** The vertex binding descriptions. */
    std::vector<VkVertexInputBindingDescription> bindingDescriptions;

    /** The vertex attribute descriptions. */
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;


    void UpdateData();
};

/**
 * Utility class to build vertex input state objects.
 */
class VertexInputInfoBuilder {
public:

    /**
     * Constructor.
     */ 
    VertexInputInfoBuilder();

    /**
     * Adds a vertex binding description.
     * 
     * @param bindingDesc   the vertex binding description to add
     * @return reference to this builder used for method chaining
     */
    VertexInputInfoBuilder& AddVertexBindingDescription(VkVertexInputBindingDescription bindingDesc);

    /**
     * Adds a vertex attribute description.
     * 
     * @param attributeDesc   the vertex attribute description to add
     * @return reference to this builder used for method chaining
     */
    VertexInputInfoBuilder& AddVertexAttributeDescription(VkVertexInputAttributeDescription attributeDesc);

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    VertexInputInfoBuilder & Reset();

    /**
     * Builds the Vulkan vertex input state structure according to the current configuration.
     * 
     * @return the vertex input state
     */
    VertexInputInfo Build() const;

private:
    /** Reserved for future use. */
    VkPipelineVertexInputStateCreateFlags flags;

    /** The vertex binding descriptions. */
    std::vector<VkVertexInputBindingDescription> bindingDescriptions;

    /** The vertex attribute descriptions. */
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;

};