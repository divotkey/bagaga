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

#include <memory>
#include <Service.h>
#include "Polyline.h"

class EntityTestService : public astu::BaseService {
public:

    /**
     * Constructor.
     */
    EntityTestService();

protected:

    // Inherited via BaseService
    virtual void OnStartup() override;
    virtual void OnShutdown() override;

private:
    std::shared_ptr<Polyline::Polygon> shape1;
};