// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes


// Local includes
#include "Multisampling.h"

using namespace std;

MultisamplingBuilder::MultisamplingBuilder()
{
    Reset();
}

MultisamplingBuilder& MultisamplingBuilder::EnableSampleShading(bool value)
{
    sampleShading = value;
    return *this;
}

MultisamplingBuilder& MultisamplingBuilder::RasterizationSamples(VkSampleCountFlagBits flags)
{
    rasterizationSamples = flags;
    return *this;
}

MultisamplingBuilder& MultisamplingBuilder::MinSampleShading(float minFraction)
{
   minSampleShading = minFraction; 
    return *this;
}

MultisamplingBuilder& MultisamplingBuilder::SampleMask(VkSampleMask *pMask)
{
    pSampleMask = pMask;
    return *this;
}

MultisamplingBuilder& MultisamplingBuilder::EnableAlphaToCoverage(bool value)
{
    alphaToCoverage = value;    
    return *this;
}

MultisamplingBuilder& MultisamplingBuilder::EnableAlphaToOne(bool value)
{
    alphaToOne = value;
    return *this;
}

MultisamplingBuilder& MultisamplingBuilder::Reset()
{
    sampleShading = false;
    rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    minSampleShading = 1.0f;
    pSampleMask = nullptr;
    alphaToCoverage = false;
    alphaToOne = false;
    return *this;
}

VkPipelineMultisampleStateCreateInfo MultisamplingBuilder::Build() const
{
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = sampleShading ? VK_TRUE : VK_FALSE;
    multisampling.rasterizationSamples = rasterizationSamples;
    multisampling.minSampleShading = minSampleShading;
    multisampling.pSampleMask = pSampleMask;
    multisampling.alphaToCoverageEnable = alphaToCoverage ? VK_TRUE : VK_FALSE;
    multisampling.alphaToOneEnable = alphaToOne ? VK_TRUE : VK_FALSE;
    return multisampling;
}
