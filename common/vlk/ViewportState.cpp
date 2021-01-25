// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
#include <stdexcept>
#include <cassert>
#include <string>
#include <iostream>

// Local includes
#include "SwapChain.h"
#include "ViewportState.h"

using namespace std;

/////////////////////////////////////////////////
/////// ViewportBuilder
/////////////////////////////////////////////////

ViewportBuilder::ViewportBuilder()
{
    Reset();
}

ViewportBuilder & ViewportBuilder::Reset()
{
    minDepth = 0.0f;
    maxDepth = 1.0f;
    width = 0;
    height = 0;
    startX = 0;
    startY = 0;
    return *this;
}

ViewportBuilder & ViewportBuilder::MinDepth(float minDepth)
{
    this->minDepth = minDepth;
    return *this;
}

ViewportBuilder & ViewportBuilder::MaxDepth(float maxDepth)
{
    this->maxDepth = maxDepth;
    return *this;
}

ViewportBuilder & ViewportBuilder::Width(float width)
{
    if (width <= 0) {
        throw std::invalid_argument("Width of viewport must be greater zero, got "
            + to_string(width));
    }

    this->width = width;
}

ViewportBuilder & ViewportBuilder::Height(float height)
{
    if (height <= 0) {
        throw std::invalid_argument("Height of viewport must be greater zero, got "
            + to_string(height));
    }

    this->height = height;
}

ViewportBuilder & ViewportBuilder::StartX(float x)
{
    if (x < 0) {
        throw std::invalid_argument("X-coordinate of viewport must be greater zero, got "
            + to_string(x));
    }

    startX = x;
    return *this;
}

ViewportBuilder & ViewportBuilder::StartY(float y)
{
    if (y < 0) {
        throw std::invalid_argument("Y-coordinate of viewport must be greater zero, got "
            + to_string(y));
    }

    startY = y;
    return *this;
}

ViewportBuilder & ViewportBuilder::ChoosePositionAndSize(const SwapChain & swapChain)
{
    StartX(0);
    StartY(0);
    Size(swapChain.GetExtent());
    return *this;
}

VkViewport ViewportBuilder::Build() const
{
    ValidateConfiguration();

    VkViewport viewport{};
    viewport.minDepth = minDepth;
    viewport.maxDepth = maxDepth;
    viewport.x = startX;
    viewport.y = startY;
    viewport.width = width;
    viewport.height = height;

    return viewport;
}

void ViewportBuilder::ValidateConfiguration() const
{
    if (width <= 0) {
        throw std::logic_error("Width of viewport must be greater zero");
    }

    if (height <= 0) {
        throw std::logic_error("Height of viewport must be greater zero");
    }
}

/////////////////////////////////////////////////
/////// ViewportStateInfo
/////////////////////////////////////////////////

ViewportStateInfo::ViewportStateInfo(
    const VkPipelineViewportStateCreateInfo & info, 
    const vector<VkViewport> & viewports,
    const vector<VkRect2D>  &scissors
    )
    : viewportState(info), viewports(viewports), scissors(scissors)
{
    UpdateAndValidate();
}

ViewportStateInfo::ViewportStateInfo(const ViewportStateInfo & o)
    : viewportState(o.viewportState)
    , viewports(o.viewports)
    , scissors(o.scissors)
{
    UpdateAndValidate();
}

ViewportStateInfo & ViewportStateInfo::operator= (const ViewportStateInfo & rhs) 
{
    viewportState = rhs.viewportState;
    viewports = rhs.viewports;
    scissors = rhs.scissors;
    UpdateAndValidate();
}


void ViewportStateInfo::UpdateAndValidate()
{
    if (viewports.size() != scissors.size()) {
        throw std::invalid_argument("Number of viewports must match number of scissors");
    }

    viewportState.viewportCount = static_cast<uint32_t>(viewports.size());
    viewportState.pViewports = viewports.data();

    viewportState.scissorCount = static_cast<uint32_t>(scissors.size());
    viewportState.pScissors = scissors.data();
}


/////////////////////////////////////////////////
/////// ViewportStateBuilder
/////////////////////////////////////////////////

ViewportStateBuilder::ViewportStateBuilder()
{
    Reset();
}

ViewportStateBuilder & ViewportStateBuilder::AddViewport(const VkViewport & viewport)
{
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent.width = viewport.width;
    scissor.extent.height = viewport.height;

    return AddViewport(viewport, scissor);
}

ViewportStateBuilder & ViewportStateBuilder::AddViewport(const VkViewport & viewport, const VkRect2D & scissor)
{
    viewports.push_back(viewport);
    scissors.push_back(scissor);
    return *this;
}

ViewportStateBuilder & ViewportStateBuilder::Reset()
{
    viewports.clear();
    scissors.clear();
    return *this;
}

ViewportStateInfo ViewportStateBuilder::Build() const
{
    assert(viewports.size() == scissors.size());

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

    // Fields will be complete by the ViewportStateInfo constructor.

    return ViewportStateInfo(viewportState, viewports, scissors);
}
