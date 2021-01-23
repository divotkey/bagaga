// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// Vulkan includes
#include <vulkan/vulkan.h>

// Vulkan includes
#include <vulkan/vulkan.h>

/**
 * Utility class used to build multisampling structures.
 */
class MultisamplingBuilder {
public:

    /**
     * Constructor.
     */
    MultisamplingBuilder();

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    MultisamplingBuilder& Reset();

    /**
     * Specifies whether sample shading should be enabled.
     * 
     * @param value `true` to enable sample shading
     * @return reference to this builder used for method chaining
     */
    MultisamplingBuilder& SampleShading(bool value);

    /**
     * Specifying the number of samples used in rasterization.
     * 
     * @param flags the rasterization samle flag bits
     * @return reference to this builder used for method chaining
     */
    MultisamplingBuilder& RasterizationSamples(VkSampleCountFlagBits flags);

    /**
     * Specifies a minimum fraction of sample shading if sample shading is enabled.
     * 
     * @param minFraction   the minimum fraction
     * @return reference to this builder used for method chaining
     */
    MultisamplingBuilder& RasterizationSamples(float minFraction);

    MultisamplingBuilder& SampleMask(VkSampleMask *pMask);

    MultisamplingBuilder& AlphaToCoverage(bool value);

    MultisamplingBuilder& AlphaToOne(bool value);
    
    /**
     * Builds the multisampling structure according to the current configuration.
     * 
     * @return the multisampling  structure
     * @throws std::logic_error in case the current configuration is invalid
     */
    VkPipelineMultisampleStateCreateInfo Build() const;

private:
    /** Enable or disables Sample Shading. */
    bool sampleShading;

    /** Specifying the number of samples used in rasterization. */
    VkSampleCountFlagBits rasterizationSamples;

    /** Specifies a minimum fraction of sample shading. */
    float minSampleShading;

    /* An array of VkSampleMask values used in the sample mask test. */
    const VkSampleMask* pSampleMask;

    /* Controls whether a temporary coverage value is generated. */
    bool alphaToCoverage;

    /** Controls whether the alpha component of the fragment’s first color output is replaced with one. */
    bool alphaToOne;
};