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
#include <EntityService.h>
#include "Pose2D.h"
#include "EntityTestService.h"



using namespace astu;

EntityTestService::EntityTestService()
    : BaseService("Entity Test")
{
    shape1 = std::make_shared<Polyline::Polygon>();  
    shape1->push_back(Vector2(-0.5, -0.5));  
    shape1->push_back(Vector2(-0.5, 0.5));  
    shape1->push_back(Vector2(0.5, 0.5));  
    shape1->push_back(Vector2(0.5, -0.5));  
}

void EntityTestService::OnStartup()
{
    auto ptr1 = std::make_unique<Pose2D>(0, 0);
    auto type1 = std::type_index(typeid(ptr1.get()));
    std::cout << "type = " << type1.name() << std::endl;
    
    auto ptr2 = std::make_shared<Pose2D>(0, 0);
    auto type2 = std::type_index(typeid(ptr1.get()));
    std::cout << "type = " << type2.name() << std::endl;

    auto entity = std::make_shared<Entity>();
    entity->AddComponent(std::make_shared<Pose2D>(0, 0));
    entity->AddComponent(std::make_shared<Polyline>(shape1));

    auto & es = GetSM().GetService<EntityService>();
    es.AddEntity(entity);
}

void EntityTestService::OnShutdown()
{
    // Intentionally left empty.
}
