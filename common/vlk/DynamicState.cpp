// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
#include <algorithm>
#include <stdexcept>

// Local includes
#include "DynamicState.h"

using namespace std;

DynamicStateBuilder::DynamicStateBuilder()
{
    Reset();
}

DynamicStateBuilder & DynamicStateBuilder::AddDynamicState(VkDynamicState state)
{
    if (HasDynamicState(state)) {
        throw logic_error("Dynamid state has already been added");
    }
    dynamicStates.push_back(state);
    return *this;
}

bool DynamicStateBuilder::HasDynamicState(VkDynamicState state) const
{
    return find(dynamicStates.begin(), dynamicStates.end(), state) != dynamicStates.end();
}

DynamicStateBuilder& DynamicStateBuilder::Reset()
{
    dynamicStates.clear();
    return *this;
}

VkPipelineDynamicStateCreateInfo DynamicStateBuilder::Build() const
{
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.empty() ? nullptr : dynamicStates.data();
    return dynamicState;
}
