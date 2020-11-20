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

#include "Vector2.h"
#include "Color.h"


/**
 * Interface for simple line rendering.
 */
class ILineRenderer {
public:

    /**
     * Virtual Destructor.
     */
    ~ILineRenderer() {}

    /**
     * Draws a line between two points.
     * 
     * @parma p1    the first point of the line
     * @parma p2    the second point of the line
     */
    virtual void DrawLine(const astu::Vector2 & p1, const astu::Vector2 & p2) {
        DrawLine(p1.x, p1.y, p2.x, p2.y);
    }

    /**
     * Draws a line between two points.
     * 
     * @parma x1    the x-coodrinate of the first point of the line
     * @parma y1    the y-coodrinate of the first point of the line
     * @parma x2    the x-coodrinate of the second point of the line
     * @parma y2    the y-coodrinate of the second point of the line
     */
    virtual void DrawLine(double x1, double y1, double x2, double X2) = 0;

    /**
     * Sets the current drawing color used for all subsequent drawing calles.
     * 
     * @param c the new drawing color 
     */
    virtual void SetDrawColor(const astu::Color & c) = 0;
};