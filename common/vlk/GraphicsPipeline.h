// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// C++ Standard Library includes
#include <memory>
#include <vector>
#include <optional>
		
// Vulkan includes
#include <vulkan/vulkan.h>

// Local includes
#include "ViewportState.h"
#include "ColorBlendState.h"
#include "ShaderStage.h"

// Forward declaration
class LogicalDevice;
class PipelineLayout;
class RenderPass;

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
     * @param layout    the pipeline layout
     */
    GraphicsPipeline(VkPipeline handle, std::shared_ptr<LogicalDevice> device, std::shared_ptr<PipelineLayout> layout);

    /**
     * Virtual destructor.
     */
    virtual ~GraphicsPipeline();

private:
    /** The handle to the Vulkan graphics pipeline object. */
    VkPipeline pipeline;

    /** The logical device this swap chain belongs to. */
    std::shared_ptr<LogicalDevice> device;

    /** The pipeline layout. */
    std::shared_ptr<PipelineLayout> layout;
};

/**
 * Utility class used to build graphics pipeline objects.
 */
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
    GraphicsPipelineBuilder & InputAssembly(const VkPipelineInputAssemblyStateCreateInfo & info);
    GraphicsPipelineBuilder & ViewportState(const ViewportStateInfo & viewportState);
    GraphicsPipelineBuilder & Rasterizer(const VkPipelineRasterizationStateCreateInfo & info);
    GraphicsPipelineBuilder & Multisampling(const VkPipelineMultisampleStateCreateInfo & info);
    GraphicsPipelineBuilder & DepthStencil(const VkPipelineDepthStencilStateCreateInfo & info);
    GraphicsPipelineBuilder & ColorBlending(const ColorBlendStateInfo & info);
    GraphicsPipelineBuilder & DynamicState(const VkPipelineDynamicStateCreateInfo & info);


    GraphicsPipelineBuilder & Layout(std::shared_ptr<PipelineLayout> layout);

    GraphicsPipelineBuilder & RenderPass(VkRenderPass renderPass);
    GraphicsPipelineBuilder & Subpass(uint32_t subpass);

    GraphicsPipelineBuilder & RenderPass(VkRenderPass renderPass, uint32_t subpass) {
        RenderPass(renderPass);
        Subpass(subpass);
        return *this;
    }

    GraphicsPipelineBuilder & BasePipeline(VkPipeline basePipeline);
    GraphicsPipelineBuilder & BasePipelineIndex(int32_t index);

    GraphicsPipelineBuilder & BasePipeline(VkPipeline basePipeline, int32_t index) {
        BasePipeline(basePipeline);
        BasePipelineIndex(index);
        return *this;
    }
    
    GraphicsPipelineBuilder & AddShaderStage(const ShaderStageInfo & stage);


    std::unique_ptr<GraphicsPipeline> Build(std::shared_ptr<LogicalDevice> device) const;

private:
    /** Describes the format of the vertex data that will be passed to the vertex shader. */
    std::optional<VkPipelineVertexInputStateCreateInfo> vertexInputInfo;

    /** Describes what kind of geometry will be rendered. */
    std::optional<VkPipelineInputAssemblyStateCreateInfo> inputAssembly;

    /** The viewport state of the pipeline to be created. */
    std::optional<ViewportStateInfo> viewportState;

    /** The rasterizer state of the pipeline to be created. */
    std::optional<VkPipelineRasterizationStateCreateInfo> rasterizer;

    /** The multisampling state of the pipeline to be created. */
    std::optional<VkPipelineMultisampleStateCreateInfo> multisampling;

    /** The deth and stencil state of the pipeline to be created. */
    std::optional<VkPipelineDepthStencilStateCreateInfo> depthStencil;

    /** The color blend state of the pipeline to be created. */
    std::optional<ColorBlendStateInfo> colorBlending;

    /** The dynamic state of the pipeline to be created. */
    std::optional<VkPipelineDynamicStateCreateInfo> dynamicState;
    
    /** The description of binding locations. */
    std::shared_ptr<PipelineLayout> pipelineLayout;
    
    /** Describes the environment in which the pipeline will be used. */
    VkRenderPass renderPass;

    /** The index of the subpass in the render pass where this pipeline will be used. */
    uint32_t subpass;

    /** The pipeline to derive from. */
    VkPipeline basePipeline;

    /** Index into the pCreateInfos parameter to use as a pipeline to derive from. */
    int32_t basePipelineIndex;

    /** The shader stages of the pipeline to create. */
    std::vector<ShaderStageInfo> shaderStages;

    /**
     * Validates the current configuration.
     * 
     * @throws std::logic_error in case the configuration is incomplete or invalid
     */
    void ValidateConfiguration() const;
};