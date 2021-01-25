// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// C++ Standard Library includes
#include <array>
#include <vector>

// Vulkan includes
#include <vulkan/vulkan.h>


/**
 * Utility class used to build blend attachment state structures.
 */
class BlendAttachmentStateBuilder {
public:

    /**
     * Constructor.
     */
    BlendAttachmentStateBuilder();

    /**
     * Enables a configuration which produces standard alpha blending.
     * 
     * @return reference to this builder used for method chaining
     */
    BlendAttachmentStateBuilder& ConfigureAlphaBlending() {
        EnableBlend(true);
        SrcColorBlendFactor(VK_BLEND_FACTOR_SRC_ALPHA);
        DstColorBlendFactor(VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA);
        ColorBlendOp(VK_BLEND_OP_ADD);
        SrcAlphaBlendFactor(VK_BLEND_FACTOR_ONE);
        DstAlphaBlendFactor(VK_BLEND_FACTOR_ZERO);
        AlphaBlendOp(VK_BLEND_OP_ADD);

        return *this;
    }

    /**
     * Specifies blending is enabled for the color attachment.
     * 
     * If blending is not enabled, the source fragment’s color for
     * that attachment is passed through unmodified.
     * 
     * @param   enable set to `true` to enable blending
     * @return reference to this builder used for method chaining
     */
    BlendAttachmentStateBuilder& EnableBlend(bool enable);

    BlendAttachmentStateBuilder& SrcColorBlendFactor(VkBlendFactor factor);
    BlendAttachmentStateBuilder& DstColorBlendFactor(VkBlendFactor factor);
    BlendAttachmentStateBuilder& ColorBlendOp(VkBlendOp blendOp);

    BlendAttachmentStateBuilder& SrcAlphaBlendFactor(VkBlendFactor factor);
    BlendAttachmentStateBuilder& DstAlphaBlendFactor(VkBlendFactor factor);
    BlendAttachmentStateBuilder& AlphaBlendOp(VkBlendOp blendOp);

    BlendAttachmentStateBuilder& ColorWriteMask(VkColorComponentFlags mask);

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    BlendAttachmentStateBuilder& Reset();

    /**
     * Builds the color blend attachment state structure according to the current configuration.
     * 
     * @return the color blend attachment state structure
     * @throws std::logic_error in case the current configuration is invalid
     */
    VkPipelineColorBlendAttachmentState Build() const;

private:
    /** Controls whether blending is enabled for the corresponding color attachment. */
    bool blendEnable;

    /** Selects which blend factor is used to determine the source factors (Sr,Sg,Sb). */
    VkBlendFactor srcColorBlendFactor;

    /** Selects which blend factor is used to determine the destination factors (Dr,Dg,Db). */
    VkBlendFactor dstColorBlendFactor;

    /** Selects which blend operation is used to calculate the RGB values to write to the color attachment. */
    VkBlendOp colorBlendOp;

    /** Selects which blend factor is used to determine the source factor Sa. */
    VkBlendFactor srcAlphaBlendFactor;

    /* selects which blend factor is used to determine the destination factor Da. */
    VkBlendFactor dstAlphaBlendFactor;

    /** selects which blend operation is use to calculate the alpha values to write to the color attachment. */
    VkBlendOp alphaBlendOp;

    /** Bitmask specifying which of the R, G, B, and/or A components are enabled for writing. */
    VkColorComponentFlags colorWriteMask;
};

/**
 * Wraps Vulkan VkPipelineColorBlendStateCreateInfo structure and
 * containts the corresponding color blend attachments structures as well.
 */
class ColorBlendStateInfo {
public:

    /**
     * Constructor.
     * 
     * The color blend state info structure will be completed by this construtor,
     * according to the content of the attachments parameter.
     * 
     * @param info          the color blend state info structure
     * @param attachments   the color blend attachments
     * @throws std::invalid_argument in case the parameters are invalid
     */
    ColorBlendStateInfo(
        const VkPipelineColorBlendStateCreateInfo & info, 
        const std::vector<VkPipelineColorBlendAttachmentState> & attachments);

    /**
     * Copy constructor.
     * 
     * @param o the other instance
     */
    ColorBlendStateInfo(const ColorBlendStateInfo & o);

    /**
     * Returns the color blend state structure.
     * 
     * @return the color blend state info structure
     */
    const VkPipelineColorBlendStateCreateInfo & GetInfo() const {
        return colorBlendState;
    }

    /**
     * Copy assignment operator.
     * 
     * @param rhs   the right hand side
     * @return reference to this instance
     */
    ColorBlendStateInfo & operator= (const ColorBlendStateInfo & rhs);

    /**
     * Implicit conversion to Vulkan handle
     */
    operator VkPipelineColorBlendStateCreateInfo() const { 
        return GetInfo();
    }

    /**
     * Implicit conversion to pointer to Vulkan VkPipelineColorBlendStateCreateInfo.
     */
    operator const VkPipelineColorBlendStateCreateInfo*() const { 
        return &GetInfo();
    }

private:
    /** The color blend state info structure. */
    VkPipelineColorBlendStateCreateInfo  colorBlendState;

    /* The color blend attachments. */
    std::vector<VkPipelineColorBlendAttachmentState> attachments;

    /**
     * Updates the color blend state structure.
     */
    void UpdateAndValidate();
};

/**
 * Utility class used to build color blend state structures.
 */
class ColorBlendStateBuilder {
public:

    /**
     * Constructor.
     */
    ColorBlendStateBuilder();

    /**
     * Specifies whether to enable 'Logical Operations'.
     * 
     * @param   enable set to `true` to enable 'Logical Operations'
     * @return reference to this builder used for method chaining
     */
    ColorBlendStateBuilder& EnableLogicOp(bool enable);

    /**
     * Selects which logical operation to apply.
     * 
     * @param logicOp   the logic operation to apply
     * @return reference to this builder used for method chaining
     */
    ColorBlendStateBuilder& LogicOp(VkLogicOp logicOp);

    /**
     * Adds a target attachment states.
     * 
     * @param attachment    the the attachment state to add
     * @return reference to this builder used for method chaining
     */
    ColorBlendStateBuilder& AddAttachment(const VkPipelineColorBlendAttachmentState attachment);

    /**
     * Specifies the components of the blend constant that are used in blending.
     * 
     * @param cR    the red component constant
     * @param cG    the Green component constant 
     * @param cB    the Blue component constant
     * @param cA    the alpha component constant
     * @return reference to this builder used for method chaining
     */
    ColorBlendStateBuilder& BlendConstants(float cR, float cG, float cB, float cA);

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    ColorBlendStateBuilder& Reset();

    /**
     * Builds the color blend state structure according to the current configuration.
     * 
     * @return the color blend state instance
     * @throws std::logic_error in case the current configuration is invalid
     */
    ColorBlendStateInfo Build() const;

private:

    /** Controls whether to apply Logical Operations.*/
    bool logicOpEnable;

    /** Selects which logical operation to apply. */
    VkLogicOp logicOp;

    /** The target attachment states. */
    std::vector<VkPipelineColorBlendAttachmentState> attachments;

    /** Array of four values used as the R, G, B, and A components. */
    std::array<float, 4> blendConstants;
};

