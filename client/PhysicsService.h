#pragma once

#include "UpdateService.h"

class PhysicsService : public astu::UpdatableBaseService {
public:

protected:

    // Inherited via UpdatableBaseService
    virtual void OnStartup() override;
    virtual void OnShutdown() override;
    virtual void OnUpdate() override;

private:
}; 