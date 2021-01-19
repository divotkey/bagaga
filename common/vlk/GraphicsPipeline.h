// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// Standard C++ Library includes
#include <memory>
#include <optional>
		

// Vulkan includes
#include <vulkan/vulkan.h>

// Forward declaration
class LogicalDevice;

/**
 * A Vulkan graphics pipeline.
 */
class GraphicsPipeline {
public:


    /**
     * Constructor.
     * 
     * @param handle    he handle to the Vulkan graphics pipeline object
     * @param device    the physical device this swap chain belongs to
     */
    GraphicsPipeline(VkPipeline handle, std::shared_ptr<LogicalDevice> device);

    /**
     * Virtual destructor.
     */
    virtual ~GraphicsPipeline();

private:
    /** The handle to the Vulkan graphics pipeline object. */
    VkPipeline pipeline;

    /** The logical device this swap chain belongs to. */
    std::shared_ptr<LogicalDevice> device;
};

class GraphicsPipelineBuilder {
public:

    /**
     * Constructor.
     */
    GraphicsPipelineBuilder();

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    GraphicsPipelineBuilder & Reset();


    GraphicsPipelineBuilder & VertexInputInfo(const VkPipelineVertexInputStateCreateInfo & info);
    GraphicsPipelineBuilder & InputAssembly(const VkPipelineInputAssemblyStateCreateInfo & assembly);

    std::unique_ptr<GraphicsPipeline> Build(std::shared_ptr<LogicalDevice> device);

private:
    /** Describes the format of the vertex data that will be passed to the vertex shader. */
    std::optional<VkPipelineVertexInputStateCreateInfo> vertexInputInfo;

    /** Describes what kind of geometry will be rendered. */
    std::optional<VkPipelineInputAssemblyStateCreateInfo> inputAssembly;

    /**
     * Validates the current configuration.
     * 
     * @throws std::logic_error in case the configuration is incomplete or invalid
     */
    void ValidateConfiguration();
};