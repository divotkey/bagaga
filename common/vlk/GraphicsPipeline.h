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
#include "VertexInputInfo.h"
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
     * Deleted copy constructor.
     */
    GraphicsPipeline(const GraphicsPipeline &) = delete;

    /**
     * Deleted assignment operator.
     */
    GraphicsPipeline & operator =(const GraphicsPipeline &) = delete;

    /**
     * Virtual destructor.
     */
    virtual ~GraphicsPipeline();

    /**
     * Returns the handle to the graphics pileline object.
     * 
     * @return the graphics pipeline handle
     */
    VkPipeline GetHandle() const {
        return pipeline;
    }

    /**
     * Implicit conversion to Vulkan handle
     */
    operator VkPipeline() const { 
        return GetHandle();
    }

private:
    /** The handle to the Vulkan graphics pipeline object. */
    VkPipeline pipeline;

    /** The logical device this graphics pipeline belongs to. */
    std::shared_ptr<LogicalDevice> device;

    /** The pipeline layout. */
    std::shared_ptr<PipelineLayout> layout;

    /**
     * Constructor.
     * 
     * @param handle    he handle to the Vulkan graphics pipeline object
     * @param device    the logical device this graphics pipeline belongs to
     * @param layout    the pipeline layout
     */
    GraphicsPipeline(VkPipeline handle, std::shared_ptr<LogicalDevice> device, std::shared_ptr<PipelineLayout> layout);

    friend class GraphicsPipelineBuilder;
};

/**
 * Utility class used to build graphics pipeline objects.
 * 
 * Note: The Vulkan API offers the option to create multiple pipelines at once, which 
 * is currently not supported by this builder utility class.
 */
class GraphicsPipelineBuilder {
public:

    /**
     * Constructor.
     */
    GraphicsPipelineBuilder();


    /**
     * Specifies how the pipeline will be generated.
     * 
     * @param flags the flags to be used to create the pipeline
     * @return reference to this builder used for method chaining
     */
    GraphicsPipelineBuilder & Flags(VkPipelineCreateFlags flags);

    /**
     * Specifies the format of the vertex data that will be passed to the vertex shader.
     * 
     * @param info  the vertex info object
     * @return reference to this builder used for method chaining
     */
    GraphicsPipelineBuilder & VertexInputState(const VertexInputInfo & info);

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


    /**
     * The pipeline to derive from.
     * 
     * @param basePipeline  the handle to the base graphics pipeline
     */        
    GraphicsPipelineBuilder & BasePipeline(VkPipeline basePipeline);
    
    GraphicsPipelineBuilder & AddShaderStage(const ShaderStageInfo & stage);

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    GraphicsPipelineBuilder & Reset();

    /**
     * Builds the graphics pipeline object according to the current configuration.
     * 
     * @param device    the logical device the graphics pipeline belongs to
     * @return the newly created graphics pipeline object
     * @throws std::runtime_exception in case the graphics pipeline object could not be created
     */
    std::unique_ptr<GraphicsPipeline> Build(std::shared_ptr<LogicalDevice> device) const;

private:
    /** Specifies how the pipeline will be generated. */
    VkPipelineCreateFlags flags;

    /** Describes the format of the vertex data that will be passed to the vertex shader. */
    std::optional<VertexInputInfo> vertexInputInfo;

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

    /** The shader stages of the pipeline to create. */
    std::vector<ShaderStageInfo> shaderStages;

    /**
     * Validates the current configuration.
     * 
     * @throws std::logic_error in case the configuration is incomplete or invalid
     */
    void ValidateConfiguration() const;
};