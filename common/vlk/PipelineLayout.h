// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// C++ Standard Library includes
#include <memory>

// Vulkan includes
#include <vulkan/vulkan.h>


/**
 * Wrapper class for Vulknan pipeline layout objects.
 */
class PipelineLayout final {
public:

    /**
     * Constructor.
     * 
     * @param handle    the handle to the pipeline layout object
     * @param device    the physical device this pipeline layout belongs to
     */
    PipelineLayout(VkPipelineLayout handle, std::shared_ptr<LogicalDevice> device);

    /**
     * Destructor.
     */
    ~PipelineLayout();

    /**
     * Returns the Vulkan handle to the pipeline layout.
     * 
     * @return the Vulkan handle
     */
    VkPipelineLayout GetHandle() const {
        return pipelineLayout;
    }

    /**
     * Implicit conversion to Vulkan handle
     */
    operator VkPipelineLayout() const { 
        return GetHandle();
    }

private:
    /** The handle to the pipeline layout object. */
    VkPipelineLayout pipelineLayout;

    /** The logical device this swap chain belongs to. */
    std::shared_ptr<LogicalDevice> device;
};

/**
 * Utility class used to build pipeline layout objects.
 */
class PipelineLayoutBuilder final {
public:

    /**
     * Constructor.
     */
    PipelineLayoutBuilder();

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    PipelineLayoutBuilder& Reset();

    /**
     * Builds the pipeline layout object according to the current configuration.
     * 
     * @param device    the logical device to use
     * @return the newly created pipeline layout
     * @throws std::logic_error in case the current configuration is invalid
     */
    std::unique_ptr<PipelineLayout> Build(std::shared_ptr<LogicalDevice> device) const;

private:
};