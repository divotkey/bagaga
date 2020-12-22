#pragma once

#include <UpdateService.h>
#include <EntityService.h>
#include "CircleCollider.h"

class CollisionDetectionSystem : 
    public astu::UpdatableBaseService
{
public:

    CollisionDetectionSystem(int priority = 0);

private:
    /** The view to the entities to be processed. */
    std::shared_ptr<astu::EntityView> entityView;

    // Inherited via Base Service
    virtual void OnStartup() override;
    virtual void OnShutdown() override;
    virtual void OnUpdate() override;

    bool IsColliding(astu::Entity & a, astu::Entity & b);
    void ReportCollision(astu::Entity & a, astu::Entity & b);
};