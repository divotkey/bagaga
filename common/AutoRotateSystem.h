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

#pragma once

#include <IteratingEntitySystem.h>
#include "ILineRenderer.h"

class AutoRotateSystem : public astu::IteratingEntitySystem {
public:

    /**
     * Constructor.
     * 
     * @param priority  the update priority of this service
     */
    AutoRotateSystem(int priority = 0);

protected:

    // Inherited via IteratingEntitySystem
    virtual void OnStartup() override;
    virtual void OnShutdown() override;
    virtual void ProcessEntity(astu::Entity & e) override;

private:
    /** A constant describing the family of entities this system processes. */
    static const astu::EntityFamily FAMILY;
};