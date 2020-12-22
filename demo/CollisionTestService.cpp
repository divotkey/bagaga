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

#define _USE_MATH_DEFINES
#include <cmath>
#include <typeindex>
#include <typeinfo>
#include <iostream>
#include <AstUtils.h>
#include <EntityService.h>
#include "IWindowManager.h"
#include "Pose2D.h"
#include "CircleCollider.h"
#include "LinearMovement.h"
#include "CollisionTestService.h"

#define ENTITY_RADIUS 30.0
#define NUM_ENTITIES 15


using namespace astu;

CollisionTestService::CollisionTestService()
    : BaseService("Entity Test")
{
    // Create circular shape.
    const int nSegments = 15;
    shape = std::make_shared<Polyline::Polygon>();  

    double da = (2 * M_PI) / nSegments;
    for(int i = 0; i < nSegments; ++i) {
        Vector2<double> v(ENTITY_RADIUS, 0);
        v.Rotate(da * i);
        shape->push_back(v);
    }
}

void CollisionTestService::OnStartup()
{

    auto & wm = GetSM().GetService<IWindowManager>();

    for(int i = 0; i <NUM_ENTITIES; ++i) {
        Vector2<double> p;
        p.x = GetRandomDouble(ENTITY_RADIUS, wm.GetWidth() - ENTITY_RADIUS);
        p.y = GetRandomDouble(ENTITY_RADIUS, wm.GetHeight() - ENTITY_RADIUS);

        AddTestEntity(p, GetRandomDouble(-180, 180), WebColors::White);
    }
}

void CollisionTestService::OnShutdown()
{
    // Intentionally left empty.
}

void CollisionTestService::AddTestEntity(const Vector2<double> & p, double s, const Color & c)
{
    Vector2<double> v(GetRandomDouble(50, 200), 0);
    v.Rotate(ToRadians(GetRandomDouble(0, 360)));

    auto entity = std::make_shared<Entity>();
    entity->AddComponent(std::make_shared<Pose2D>(p));
    entity->AddComponent(std::make_shared<Polyline>(shape, c));
    entity->AddComponent(std::make_shared<LinearMovement>(v));
    entity->AddComponent(std::make_shared<CircleCollider>(ENTITY_RADIUS));

    auto & es = GetSM().GetService<EntityService>();
    es.AddEntity(entity);
}
