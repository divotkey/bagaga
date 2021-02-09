// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// C++ Standard Library includes
#include <memory>

// Forward declaration;
class IVulkanRenderLayer;
class SwapChain;
class PhysicalDevice;
class LogicalDevice;
class GraphicsPipeline;
class RenderPass;

/** 
 * Interface for Vulkan render systems.
 * 
 * A Vulkan render system is responsible to create Vulkan-based render output infrastructure.
 * The managed render layers submits commands to the graphics queue of a logical Vulkan device.
 */
class IVulkanRenderSystem {
public:

    /**
     * Virtual Destructor.
     */
    virtual ~IVulkanRenderSystem() {}

    /**
     * Returns the swap chain used by this render system.
     * 
     * @return the swap chain
     */
    virtual const SwapChain & GetSwapChain() const = 0;

    /**
     * Returns the render pass object used by this render system.
     * 
     * @return the render pass object
     */
    virtual const RenderPass & GetRenderPass() const = 0;

    /**
     * Returns the physical device used by this render system.
     * 
     * @return the physical device
     */
    virtual const PhysicalDevice & GetPhysicalDevice() const = 0;

    /**
     * Returns the base graphics pipeline to be used to create derived pipelines.
     * 
     * @return the base graphics pipeline
     */
    virtual const GraphicsPipeline & GetBaseGraphicsPipeline() const = 0;

    /**
     * Returns the logical device used by this render system.
     * 
     * @return the logical device
     */
    virtual std::shared_ptr<const LogicalDevice> GetLogicalDevice() const = 0;    

    /**
     * Returns the logical device used by this render system.
     * 
     * @return the logical device
     */
    virtual std::shared_ptr<LogicalDevice> GetLogicalDevice() = 0;    

    /**
     * Adds a render layer to this renderer.
     * 
     * @param layer the render layer to add
     * @throws std::logic_error in case the render layer has already been added
     */
    virtual void AddRenderLayer(std::shared_ptr<IVulkanRenderLayer> layer) = 0;

    /**
     * Tests whether a render layer has already been added.
     * 
     * @param layer the layer to test
     * @return `true` if the layer has already been added
     */
    virtual bool HasRenderLayer(std::shared_ptr<IVulkanRenderLayer> layer) = 0;

    /**
     * Removes a render layer from this renderer.
     * 
     * IF the render layer is unknown, this method has no effect.
     * 
     * @param layer the render layer to remove
     */
    virtual void RemoveRenderLayer(std::shared_ptr<IVulkanRenderLayer> layer) = 0;

};