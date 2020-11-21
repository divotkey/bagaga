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

#include <typeindex>
#include <typeinfo>
#include <iostream>
#include <AstUtils.h>
#include <EntityService.h>
#include "IWindowManager.h"
#include "Pose2D.h"
#include "AutoRotate.h"
#include "EntityTestService.h"

#define ENTITY_SIZE 30.0
#define NUM_ENTITIES 25


using namespace astu;

EntityTestService::EntityTestService()
    : BaseService("Entity Test")
{
    // Create rectangular shape.
    shape1 = std::make_shared<Polyline::Polygon>();  
    shape1->push_back(Vector2(-ENTITY_SIZE, -ENTITY_SIZE));  
    shape1->push_back(Vector2(-ENTITY_SIZE, ENTITY_SIZE));  
    shape1->push_back(Vector2(ENTITY_SIZE, ENTITY_SIZE));  
    shape1->push_back(Vector2(ENTITY_SIZE, -ENTITY_SIZE));  

    // Create triangular shape.
    shape2 = std::make_shared<Polyline::Polygon>();  
    shape2->push_back(Vector2(-ENTITY_SIZE, -ENTITY_SIZE));  
    shape2->push_back(Vector2(ENTITY_SIZE, -ENTITY_SIZE));  
    shape2->push_back(Vector2(0, ENTITY_SIZE));  
}

void EntityTestService::OnStartup()
{

    auto & wm = GetSM().GetService<IWindowManager>();

    double r = sqrt(ENTITY_SIZE * ENTITY_SIZE * 2);
    for(int i = 0; i <NUM_ENTITIES; ++i) {
        Vector2 p;
        p.x = GetRandomDouble(r, wm.GetWidth() - r);
        p.y = GetRandomDouble(r, wm.GetHeight() - r);

        Color c;
        c.r = GetRandomDouble(0.25, 1);
        c.g = GetRandomDouble(0.25, 1);
        c.b = GetRandomDouble(0.25, 1);
        AddTestEntity(GetRandomInt(1, 3), p, GetRandomDouble(-180, 180), c);
    }
}

void EntityTestService::OnShutdown()
{
    // Intentionally left empty.
}


void EntityTestService::AddTestEntity(int t, const Vector2 & p, double s, const Color & c)
{
    auto entity = std::make_shared<Entity>();
    entity->AddComponent(std::make_shared<Pose2D>(p));
    entity->AddComponent(std::make_shared<Polyline>(t == 1 ? shape1 : shape2, c));
    entity->AddComponent(std::make_shared<AutoRotate>(ToRadians(s)));

    auto & es = GetSM().GetService<EntityService>();
    es.AddEntity(entity);
}
