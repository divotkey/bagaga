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
#include "Pose2D.h"
#include "AutoRotate.h"
#include "AutoRotateSystem.h"

using namespace astu;

const EntityFamily AutoRotateSystem::FAMILY = EntityFamily::Create<Pose2D, AutoRotate>();

AutoRotateSystem::AutoRotateSystem(int priority)
    : IteratingEntitySystem(FAMILY, priority, "AutoRotate System")
{
    // Intentionally left empty.
}

void AutoRotateSystem::OnStartup()
{
    // Intentionally left empty.
}

void AutoRotateSystem::OnShutdown()
{
    // Intentionally left empty.
}


void AutoRotateSystem::ProcessEntity(Entity & e)
{
    auto & pose = e.GetComponent<Pose2D>();
    auto & rotate = e.GetComponent<AutoRotate>();

    pose.angle += rotate.speed * GetDeltaTime();
}
