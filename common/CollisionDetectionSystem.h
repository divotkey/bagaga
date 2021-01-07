#pragma once

#include <UpdateService.h>
#include <EntityService.h>
#include <SignalService.h>
#include "CircleCollider.h"


class CollisionEvent final {
public:
    std::shared_ptr<astu::Entity> entityA;
    std::shared_ptr<astu::Entity> entityB;

    CollisionEvent(std::shared_ptr<astu::Entity> a, std::shared_ptr<astu::Entity> b)
        : entityA(a), entityB(b)
    {
        // Intentionally left empty.
    }

    astu::Entity & GetEntityA() const { 
        return *entityA;
    }

    astu::Entity & GetEntityB() const {
        return *entityB;
    }
};

using CollisionEventService = astu::SignalService<CollisionEvent>;
using CollisionListener = astu::ISignalListener<CollisionEvent>;


class CollisionDetectionSystem : 
    public astu::UpdatableBaseService
{
public:

    CollisionDetectionSystem(int priority = 0);

private:
    /** The view to the entities to be processed. */
    std::shared_ptr<astu::EntityView> entityView;

    /** Used to report collisions. */
    std::shared_ptr<CollisionEventService> collisionEventService;

    // Inherited via Base Service
    virtual void OnStartup() override;
    virtual void OnShutdown() override;
    virtual void OnUpdate() override;

    bool IsColliding(astu::Entity & a, astu::Entity & b);
    void ReportCollision(std::shared_ptr<astu::Entity> a, std::shared_ptr<astu::Entity> b);
};