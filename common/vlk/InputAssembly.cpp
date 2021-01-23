// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
#include <cassert>

// Local includes
#include "InputAssembly.h"

using namespace std;

InputAssemblyBuilder::InputAssemblyBuilder()
{
    Reset();
}

InputAssemblyBuilder & InputAssemblyBuilder::Reset()
{
    topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    primitiveRestart = false;
    return *this;
}

InputAssemblyBuilder & InputAssemblyBuilder::Topology(VkPrimitiveTopology topology)
{
    this->topology = topology;
    return *this;
}

InputAssemblyBuilder & InputAssemblyBuilder::PrimitiveRestart(bool enable)
{
    primitiveRestart = enable;
    return *this;
}

VkPipelineInputAssemblyStateCreateInfo InputAssemblyBuilder::Build() const
{
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    inputAssembly.topology = topology;
    inputAssembly.primitiveRestartEnable = primitiveRestart ? VK_TRUE : VK_FALSE;

    return inputAssembly;
}
