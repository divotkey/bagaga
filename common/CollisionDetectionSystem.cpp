#include <EntityService.h>
#include <Vector2.h>
#include <iostream>
#include "Pose2D.h"
#include "CircleCollider.h"
#include "Polyline.h"
#include "CollisionDetectionSystem.h"

using namespace astu;

CollisionDetectionSystem::CollisionDetectionSystem(int priority)
    : UpdatableBaseService("Collision Detection", priority)
{
    // Intentionally left empty.
}

void CollisionDetectionSystem::OnStartup()
{
    auto & es = GetSM().GetService<EntityService>();
    entityView = 
        es.GetEntityView(EntityFamily::Create<Pose2D, CircleCollider>());
}

void CollisionDetectionSystem::OnShutdown()
{
    entityView = nullptr;    
}

void CollisionDetectionSystem::OnUpdate()
{

    // Debug start
    for (auto & e : *entityView) {
        e->GetComponent<Polyline>().color = WebColors::White;
    }
    // Debug end

    for (size_t j = 0; j < entityView->size(); ++j) {
        const auto & entityA = (*entityView)[j];

        for (size_t i = j + 1; i < entityView->size(); ++i) {
            const auto & entityB = (*entityView)[i];

            if (IsColliding(*entityA, *entityB)) {
                // Report collision.
                ReportCollision(*entityA, *entityB);
                // std::cout << "Collision!" << std::endl;
            }
        }
    }
}

bool CollisionDetectionSystem::IsColliding(astu::Entity & a, astu::Entity & b)
{
    // Get components of entity A
    auto & poseA = a.GetComponent<Pose2D>();
    auto & colA = a.GetComponent<CircleCollider>();

    // Get components of entity B
    auto & poseB = b.GetComponent<Pose2D>();
    auto & colB = b.GetComponent<CircleCollider>();

    Vector2<double> d = poseA.pos - poseB.pos;

    double radiusSum = colA.radius+ colB.radius;
    return d.LengthSquared() <= radiusSum * radiusSum;
}

void CollisionDetectionSystem::ReportCollision(astu::Entity & a, astu::Entity & b)
{
    // Debug version.
    auto & polyA = a.GetComponent<Polyline>();
    auto & polyB = b.GetComponent<Polyline>();

    polyA.color = WebColors::Red;
    polyB.color = WebColors::Red;
}
