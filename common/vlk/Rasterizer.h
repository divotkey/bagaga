// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// Vulkan includes
#include <vulkan/vulkan.h>

/**
 * Utility class used to build rasterizer structures.
 */
class RasterizerBuilder {
public:

    /**
     * Constructor.
     */
    RasterizerBuilder();


    /**
     * Specified whether to enable depth clamp.
     * 
     * If depthClamp is enabled, then fragments that are beyond the near and
     * far planes are clamped to them as opposed to discarding them.
     * This is useful in some special cases like shadow maps.
     * Using this requires enabling a GPU feature.
     * 
     * @param value `true` to enable depth clamp.
     * @return reference to this builder used for method chaining
     */
    RasterizerBuilder & EnableDepthClamp(bool value);

    /**
     * Specified primitives are discarded immediately before the rasterization stage.
     *      * 
     * @param value `true` to enable rasterizer discard.
     * @return reference to this builder used for method chaining
     */
    RasterizerBuilder & EnableRasterizerDiscard(bool value);

    /**
     * Specifies the polygon rasterization mode.
     * 
     * @param mode  the polygon rasterization mode
     * @return reference to this builder used for method chaining
     */
    RasterizerBuilder & PolygonMode(VkPolygonMode mode);

    /**
     * Specifies the width of rasterized line segments.
     * 
     * The maximum line width that is supported depends on the hardware and
     * any line thicker than 1.0f requires you to enable the wideLines GPU
     * feature.
     * 
     * @param width the width of line segments
     * @return reference to this builder used for method chaining
     */
    RasterizerBuilder & LineWidth(float width);

    /**
     * Specifies the the type of face culling to use.
     *      
     * @param flags the cull mode flags
     * @return reference to this builder used for method chaining
     */
    RasterizerBuilder & CullMode(VkCullModeFlags flags);

    /**
     * Specifies the vertex order for faces.
     *      
     * @param frontFace the vertex order
     * @return reference to this builder used for method chaining
     */
    RasterizerBuilder & FrontFace(VkFrontFace frontFace);


    RasterizerBuilder & EnableDepthBias(bool value);

    RasterizerBuilder & DepthBiasConstantFactor(float constantFactor);

    RasterizerBuilder & DepthBiasClamp(float biasClamp);

    RasterizerBuilder & DepthBiasSlopeFactor(float slopeFactor);

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    RasterizerBuilder& Reset();

    /**
     * Builds the rasterizer structure according to the current configuration.
     * 
     * @return the rasterizer structure
     * @throws std::logic_error in case the current configuration is invalid
     */
    VkPipelineRasterizationStateCreateInfo Build() const;

private:

    /** Whether to enable depth clamp of fragments. */
    bool depthClamp;

    /** Controls whether primitives are discarded immediately before the rasterization stage. */
    bool rasterizerDiscard;

    /** Control polygon rasterization mode. */
    VkPolygonMode polygonMode;

    /** The width of rasterized line segments.*/
    double lineWidth;

    /** Determines the type of face culling to use. */
    VkCullModeFlags cullMode;

    /** Specifies the vertex order for faces. */
    VkFrontFace frontFace;

    /** Controls whether to bias fragment depth values.*/
    bool depthBias;

    /** A scalar factor controlling the constant depth value added to each fragment. */
    float depthBiasConstantFactor;

    /** The maximum (or minimum) depth bias of a fragment. */
    float depthBiasClamp;

    /** A scalar factor applied to a fragment’s slope in depth bias calculations. */
    float depthBiasSlopeFactor;
};