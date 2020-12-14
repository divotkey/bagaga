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

class Pose2D : public astu::EntityComponent {
public:
    astu::Vector2<double> pos;
    double angle;

    Pose2D(double x = 0, double y = 0, double a = 0)
        : pos(x, y)
        , angle(a)
    {
        // Intentionally left empty.            
    }

   Pose2D(const astu::Vector2<double> & p, double a = 0)
        : pos(p)
        , angle(a)
    {
        // Intentionally left empty.            
    }
};