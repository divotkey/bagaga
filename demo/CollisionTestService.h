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

#include <memory>
#include <Service.h>
#include "Polyline.h"

class CollisionTestService : public astu::BaseService {
public:

    /**
     * Constructor.
     */
    CollisionTestService();

protected:

    // Inherited via BaseService
    virtual void OnStartup() override;
    virtual void OnShutdown() override;

private:
    std::shared_ptr<Polyline::Polygon> shape;

    /**
     * Adds a test entity at a certain position.
     * 
     * @param p the position of the test entity in world space
     * @param s the rotation speed in degrees per seconds
     * @param c the color of the test entity
     */
    void AddTestEntity(const astu::Vector2<double> & p, double s, const astu::Color & c);
};