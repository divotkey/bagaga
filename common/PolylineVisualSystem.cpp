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
#include "Polyline.h"
#include "PolylineVisualSystem.h"

using namespace astu;

const EntityFamily PolylineVisualSystem::FAMILY = EntityFamily::Create<Pose2D, Polyline>();

PolylineVisualSystem::PolylineVisualSystem(int priority)
    : IteratingEntitySystem(FAMILY, priority, "Polyline Visual System")
{
    // Intentionally left empty.
}

void PolylineVisualSystem::OnStartup()
{
    renderer = GetSM().FindService<ILineRenderer>();
    if (!renderer) {
        throw std::logic_error("ILineRenderer required for Polyline Visual System");
    }
}

void PolylineVisualSystem::OnShutdown()
{
    renderer = nullptr;
}


void PolylineVisualSystem::ProcessEntity(Entity & e)
{
    auto & pose = e.GetComponent<Pose2D>();
    auto  & poly = e.GetComponent<Polyline>();

    renderer->SetDrawColor(poly.color);

    auto & polygon = *poly.polygon;
    assert(polygon.size() >= 2);

    Vector2* ptr = polygon.data();
    Vector2 p1 = *ptr++;
    p1.Rotate(pose.angle);
    p1 += pose.pos;

    for (size_t i = 0; i < polygon.size() - 1; ++i) {
        Vector2 p2 = *ptr++;
        p2.Rotate(pose.angle);
        p2 += pose.pos;
        renderer->DrawLine(p1, p2);
        p1 = p2;
    }

    if (poly.closed) {
        Vector2 p2 = *polygon.data();
        p2.Rotate(pose.angle);
        p2 += pose.pos;
        renderer->DrawLine(p1, p2);
    }

}
