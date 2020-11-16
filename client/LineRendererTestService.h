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

#include <Vector2.h>
#include "ITimeService.h"
#include "ILineRenderer.h"
#include "UpdateService.h"


class LineRendererTestService : public astu::UpdatableBaseService {
public:

protected:

    // Inherited via UpdatableBaseService
    virtual void OnStartup() override;
    virtual void OnShutdown() override;
    virtual void OnUpdate() override;

private:
    /** Used for fast access to line render service. */
    std::shared_ptr<ILineRenderer> lineRenderer;

    /** Used for fast access to time service. */
    std::shared_ptr<astu::ITimeService> timeService;

    /** The width of the output window. */
    int width;

    /** The height of the output window. */
    int height;

    class MovingLine {
    public:
        /** Constructor. */
        MovingLine(double width, double height);

        /** The first point of the moving line. */
        astu::Vector2 p1;

        /** The second point of the moving line. */
        astu::Vector2 p2;

        /** The velocity of the first end point. */
        astu::Vector2 v1;

        /** The velocity of the second end point. */
        astu::Vector2 v2;

    };

    std::vector<MovingLine> lines;

    void UpdateLine(MovingLine& line, double dt);
    void RenderLine(MovingLine& line);
    void KeepWithinBoundaries(astu::Vector2 & p, astu::Vector2 & v);
}; 