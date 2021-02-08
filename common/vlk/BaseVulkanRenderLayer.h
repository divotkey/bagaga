// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// AST Utilities includes.
#include <Service.h>

// Local includes
#include "IVulkanRenderLayer.h"

class BaseVulkanRenderLayer : public astu::BaseService, public IVulkanRenderLayer {
public:

    /**
     * Constructor.
     * 
     * @param renderPriority    the render priority of this layer
     */
    BaseVulkanRenderLayer(const std::string & name = DEFAULT_NAME, int renderPriority = 0);

    // Inherited via IVulkanRenderLayer
    virtual int GetRenderPriority() const final;

    // Inherited via BaseService
    virtual void OnStartup() override;
    virtual void OnShutdown() override;

private:
    /** The render priority of this layer. */
    int renderPriority;
};
