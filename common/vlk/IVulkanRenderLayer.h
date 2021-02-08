// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once


// Forward declaration
class CommandBuffer;

/** 
 * Interface for Vulkan render layers.
 */
class IVulkanRenderLayer {
public:

    /**
     * Virtual Destructor.
     */
    virtual ~IVulkanRenderLayer() {}

    /**
     * Returns the priority used to sort render layers.
     * 
     * Render layers with higher priority values are render after layers
     * with lower priority.
     * 
     * @return the render priority.
     */
    virtual int GetRenderPriority() const = 0;

    /**
     * Called to emit render commands.
     * 
     * @param cmdBuf    the command buffer used to emit render commands
     */
    virtual void Render(CommandBuffer & cmdBuf) = 0;

};