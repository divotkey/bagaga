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

#include <stdexcept>
#include <cassert>
#include <IWindowManager.h>
#include "Pose2D.h"
#include "LinearMovement.h"
#include "LinearMovementSystem.h"

using namespace astu;

const EntityFamily LinearMovementSystem::FAMILY = EntityFamily::Create<Pose2D, LinearMovement>();

LinearMovementSystem::LinearMovementSystem(int priority)
    : IteratingEntitySystem(FAMILY, priority, "AutoRotate System")
{
    // Intentionally left empty.
}

void LinearMovementSystem::OnStartup()
{
    auto & wm = GetSM().GetService<astu::IWindowManager>();
    width = wm.GetWidth();
    height = wm.GetHeight();
}

void LinearMovementSystem::OnShutdown()
{
    // Intentionally left empty.
}

void LinearMovementSystem::ProcessEntity(Entity & e)
{
    auto & pose = e.GetComponent<Pose2D>();
    auto & mov = e.GetComponent<LinearMovement>();

    pose.pos += mov.vel * GetDeltaTime();

    // Keep within boundaries.
    if (pose.pos.x < 0) {
        pose.pos.x = 0;
        mov.vel.x = -mov.vel.x;
    }
    if (pose.pos.x >= width) {
        pose.pos.x = width - 1;
        mov.vel.x = -mov.vel.x;
    }

    if (pose.pos.y < 0)  {
        pose.pos.y = 0;
        mov.vel.y = -mov.vel.y;
    }
    if (pose.pos.y >= height) {
        pose.pos.y = height - 1;
        mov.vel.y = -mov.vel.y;
    }
}
