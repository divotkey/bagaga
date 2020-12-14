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

#include <cassert>
#include <typeindex>
#include <typeinfo>
#include <iostream>
#include <AstUtils.h>
#include <EntityService.h>
#include <Mouse.h>
#include "IWindowManager.h"
#include "Pose2D.h"
#include "AutoRotate.h"
#include "CreateEntityTestService.h"

#define ENTITY_SIZE 30.0
#define NUM_ENTITIES 25

using namespace astu;

CreateEntityTestService::CreateEntityTestService(int updatePriority)
    : UpdatableBaseService("Create Entity Test", updatePriority)
{
    // Create rectangular shape.
    shape1 = std::make_shared<Polyline::Polygon>();  
    shape1->push_back(Vector2<double>(-ENTITY_SIZE, -ENTITY_SIZE));  
    shape1->push_back(Vector2<double>(-ENTITY_SIZE, ENTITY_SIZE));  
    shape1->push_back(Vector2<double>(ENTITY_SIZE, ENTITY_SIZE));  
    shape1->push_back(Vector2<double>(ENTITY_SIZE, -ENTITY_SIZE));  

    // Create triangular shape.
    shape2 = std::make_shared<Polyline::Polygon>();  
    shape2->push_back(Vector2<double>(-ENTITY_SIZE, -ENTITY_SIZE));  
    shape2->push_back(Vector2<double>(ENTITY_SIZE, -ENTITY_SIZE));  
    shape2->push_back(Vector2<double>(0, ENTITY_SIZE));  
}

void CreateEntityTestService::OnStartup()
{
    GetSM().GetService<MouseButtonEventService>()
        .AddListener(shared_as<CreateEntityTestService>());
}

void CreateEntityTestService::OnShutdown()
{
    GetSM().GetService<MouseButtonEventService>()
        .RemoveListener(shared_as<CreateEntityTestService>());
}

void CreateEntityTestService::OnUpdate()
{
    // Mouse mouse;

    // if (mouse.IsPressed(3)) {
    //     Vector2 pos(mouse.GetCursorX(), mouse.GetCursorY());
    //     AddTestEntity(1, pos, 100, WebColors::White);
    // }
}

void CreateEntityTestService::OnSignal(const MouseButtonEvent & signal)
{
    Mouse mouse;

    if (signal.button == MouseButtonEvent::BUTTON::RIGHT && signal.pressed) {
        Vector2<double> pos(mouse.GetCursorX(), mouse.GetCursorY());
        AddTestEntity(1, pos, 100, WebColors::White);
    }
}

void CreateEntityTestService::AddTestEntity(int t, const Vector2<double> & p, double s, const Color & c)
{
    auto entity = std::make_shared<Entity>();
    entity->AddComponent(std::make_shared<Pose2D>(p));
    entity->AddComponent(std::make_shared<Polyline>(t == 1 ? shape1 : shape2, c));
    entity->AddComponent(std::make_shared<AutoRotate>(ToRadians(s)));

    auto & es = GetSM().GetService<EntityService>();
    es.AddEntity(entity);
}
