// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// Local includes
#include "IVulkanRenderSystem.h"
#include "BaseVulkanRenderLayer.h"

BaseVulkanRenderLayer::BaseVulkanRenderLayer(const std::string & name, int renderPriority)
    : BaseService(name)
    , renderPriority(renderPriority)
{
    // Intentionally left empty.
}

int BaseVulkanRenderLayer::GetRenderPriority() const
{
    return renderPriority;
}

void BaseVulkanRenderLayer::OnStartup() 
{
    GetSM().GetService<IVulkanRenderSystem>()
        .AddRenderLayer(shared_as<IVulkanRenderLayer>());
}

void BaseVulkanRenderLayer::OnShutdown()
{
    GetSM().GetService<IVulkanRenderSystem>()
        .RemoveRenderLayer(shared_as<IVulkanRenderLayer>());
}
