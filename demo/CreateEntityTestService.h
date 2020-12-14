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
#include <UpdateService.h>
#include <Events.h>
#include "Polyline.h"

class CreateEntityTestService 
    : public astu::UpdatableBaseService
    , public astu::MouseButtonListener
{
public:

    /**
     * Constructor.
     */
    CreateEntityTestService(int updatePriority = 0);

protected:

    // Inherited via BaseService
    virtual void OnStartup() override;
    virtual void OnShutdown() override;
    virtual void OnUpdate() override;

    // Inherited via MouseButtonListener
    virtual void OnSignal(const astu::MouseButtonEvent & signal) override;  

private:
    std::shared_ptr<Polyline::Polygon> shape1;
    std::shared_ptr<Polyline::Polygon> shape2;

    /**
     * Adds a test entity at a certain position.
     * 
     * @param t the type of the entity to create
     * @param p the position of the test entity in world space
     * @param s the rotation speed in degrees per seconds
     * @param c the color of the test entity
     */
    void AddTestEntity(int t, const astu::Vector2<double> & p, double s, const astu::Color & c);
};