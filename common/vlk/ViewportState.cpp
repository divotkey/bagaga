// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
#include <stdexcept>
#include <cassert>
#include <string>

// Local includes
#include "SwapChain.h"
#include "ViewportState.h"

using namespace std;

ViewportStateBuilder::ViewportStateBuilder()
{
    Reset();
}

ViewportStateBuilder & ViewportStateBuilder::MinDepth(float minDepth)
{
    this->minDepth = minDepth;
    return *this;
}

ViewportStateBuilder & ViewportStateBuilder::MaxDepth(float maxDepth)
{
    this->maxDepth = maxDepth;
    return *this;
}

ViewportStateBuilder & ViewportStateBuilder::Width(float width)
{
    if (width <= 0) {
        throw std::invalid_argument("Width of viewport must be greater zero, got "
            + to_string(width));
    }

    this->width = width;
}

ViewportStateBuilder & ViewportStateBuilder::Height(float height)
{
    if (width <= 0) {
        throw std::invalid_argument("Height of viewport must be greater zero, got "
            + to_string(height));
    }

    this->height = height;
}

ViewportStateBuilder & ViewportStateBuilder::StartX(float x)
{
    if (x < 0) {
        throw std::invalid_argument("X-coordinate of viewport must be greater zero, got "
            + to_string(x));
    }

    startX = x;
    return *this;
}

ViewportStateBuilder & ViewportStateBuilder::StartY(float y)
{
    if (y < 0) {
        throw std::invalid_argument("Y-coordinate of viewport must be greater zero, got "
            + to_string(y));
    }

    startX = y;
    return *this;
}

ViewportStateBuilder & ViewportStateBuilder::ChoosePositionAndSize(const SwapChain & swapChain)
{
    StartX(0);
    StartY(0);
    Size(swapChain.GetExtent());
    return *this;
}

ViewportStateBuilder & ViewportStateBuilder::Reset()
{
    minDepth = 0.0f;
    maxDepth = 1.0f;
    width = 0;
    height = 0;
    startX = 0;
    startY = 0;
    return *this;
}

VkPipelineViewportStateCreateInfo ViewportStateBuilder::Build() const
{
    ValidateConfiguration();

    VkViewport viewport{};
    viewport.minDepth = minDepth;
    viewport.maxDepth = maxDepth;
    viewport.x = startX;
    viewport.y = startY;
    viewport.width = width;
    viewport.height = height;

    // TODO make configurable.
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent.width = width;
    scissor.extent.height = height;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    return viewportState;
}

void ViewportStateBuilder::ValidateConfiguration() const
{
    if (width <= 0) {
        throw std::logic_error("Width of viewport must be greater zero");
    }

    if (height <= 0) {
        throw std::logic_error("Height of viewport must be greater zero");
    }
}
