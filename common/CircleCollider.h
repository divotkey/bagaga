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

#include <EntityService.h>
#include <Vector2.h>

class CircleCollider : public astu::EntityComponent {
public:
    /** The radius of the circle collider. */
    double radius;

    /**
     * Constructor.
     * 
     * @param r the radius
     */
    CircleCollider(double r = 1.0)
        : radius(r)
    {
        // Intentionally left empty.
    }

};