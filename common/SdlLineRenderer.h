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

#include <SdlRenderService.h>
#include <vector>
#include <cstdint>
#include "ILineRenderer.h"

/**
 * A SDL-based implementation of the ILineRenderer interface.
 * 
 * This service is a SDL render layer and uses the command design pattern
 * to store the render calls and replays them when the render layer should
 * be rendered.
 */
class SdlLineRenderer : public astu::BaseSdlRenderLayer, public ILineRenderer {
public:

    /**
     * Constructor.
     * 
     * @param renderPriority    the priority of this render layer
     */
    SdlLineRenderer(int renderPriority = 0);

    /**
     * Virtual destructor.
     */
    virtual ~SdlLineRenderer() {}

    // Inherited via BaseSdlRenderLayer
    virtual void OnRender(SDL_Renderer* renderer) override;

    // Inherited via ILineRenderer
    virtual void DrawLine(double x1, double y1, double x2, double X2) override;
    virtual void SetDrawColor(const astu::Color & c) override;

protected:

    // Inherited via BaseSdlRenderLayer
    virtual void OnStartup() override;
    virtual void OnShutdown() override;

private:

    /** Enumeration for types of render commands. */
    enum CommandType {DRAW_LINE, SET_COLOR};

    struct DrawLineCommand {
        CommandType type;
        int x1, y1, x2, y2;
    };

    struct SetColorCommand {
        CommandType type;
        int r, g, b, a;
    };

    /** Union holding data for all types of render commands. */
    union RenderCommand {
        CommandType type;
        DrawLineCommand line;
        SetColorCommand color;
    };

    // /** The current render commands to be processed. */
    std::vector<RenderCommand> commands;
};