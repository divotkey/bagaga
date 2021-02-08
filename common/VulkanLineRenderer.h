/*  ____          _____          _____          
 * |  _ \   /\   / ____|   /\   / ____|   /\    
 * | |_) | /  \ | |  __   /  \ | |  __   /  \   
 * |  _ < / /\ \| | |_ | / /\ \| | |_ | / /\ \  
 * | |_) / ____ \ |__| |/ ____ \ |__| |/ ____ \ 
 * |____/_/    \_\_____/_/    \_\_____/_/    \_\
 *
 * Bagaga - Bloody Amazing Game Architecture Game
 * Copyright 2020 Bagaga Development Team. All rights reserved.                                             
 */

#pragma once

// C++ Standard Library includes
#include <memory>

// AST Utilities includes
#include <Vector2.h>
#include <Vector3.h>
#include <Color.h>
#include <Matrix3.h>

// Local includes.
#include "vlk/BaseVulkanRenderLayer.h"
#include "ILineRenderer.h"

// Forward declarations.
class IVulkanRenderSystem;
class GraphicsPipeline;
class Buffer;

class VulkanLineRenderer : public BaseVulkanRenderLayer, public ILineRenderer {
public:

    /**
     * Constructor.
     * 
     * @param maxLines          the maximum number of lines to be drawn
     * @param renderPriority    the render priority of this layer
     */
    VulkanLineRenderer(size_t maxLines = 5000, int renderPriority = 0);

    /**
     * Destructor.
     */
    virtual ~VulkanLineRenderer();

    // Inherited via BaseVulkanRenderLayer
    virtual void Render(CommandBuffer & cmdBuf) override;
    virtual void OnStartup() override;
    virtual void OnShutdown() override;

    // Inherited via ILineRenderer
    virtual void DrawLine(double x1, double y1, double x2, double y2) override;
    virtual void SetDrawColor(const astu::Color & c) override;


private:

    /**
     * The attributes of the line vertices render.
     */
    struct LineVertex {

        LineVertex(float x, float y, float r, float g, float b)
            : pos(x, y), col(r, g, b) {}

        LineVertex(astu::Vector2<float> p, float r, float g, float b)
            : pos(p), col(r, g, b) {}


        astu::Vector2<float> pos;
        astu::Vector3<float> col;
    };

    /** The graphics pipeline used to render lines. */
    std::unique_ptr<GraphicsPipeline> graphicsPipeline;

    /** The Vulkan buffer object holding the line vertices. */
    std::unique_ptr<Buffer> vertexBuffer;

    /** The vertices of the lines to draw. */
    std::vector<LineVertex> vertices;

    /** The transformation applied to the vertices. */
    astu::Matrix3<float> transform;

    /** The current draw color for lines. */
    astu::Color drawColor;

    /**
     * Creates the graphics pipeline for the line rendering.
     * 
     * @param rndSys    the render system used to create the graphics pipeline
     * @throws std::runtime_error in case the graphics pipeline could not be created
     */
    void CreateGraphicsPipeline(IVulkanRenderSystem & renderSys);

    /**
     * Creates the vertex buffer used to render the lines.
     * 
     * @param rndSys    the render system used to create vertex buffer
     * @throws std::runtime_error in case the vertex buffer could not be created
     */
    void CreateVertexBuffer(IVulkanRenderSystem & renderSys);
};
