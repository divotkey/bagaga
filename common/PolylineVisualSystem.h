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

class PolylineVisualSystem : astu::IteratingEntitySystem {
public:

    /**
     * Constructor.
     * 
     * @param priority  the update priority of this service
     */
    PolylineVisualSystem(int priority = 0);

protected:

        // Inherited via IteratingEntitySystem
        virtual void Startup() override;
        virtual void Shutdown() override;
        virtual void ProcessEntity(astu::Entity & e) override;

private:
    /** A constant describing the family of entities this system processes. */
    static const astu::EntityFamily FAMILY;

    /** The line renderer used to render the visuals. */
    std::shared_ptr<ILineRenderer> renderer;
};