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

#include "ILineRenderer.h"
#include "UpdateService.h"


class LineRendererTestService : public astu::UpdatableBaseService {
public:

protected:

    // Inherited via UpdatableBaseService
    virtual void OnStartup() override;
    virtual void OnShutdown() override;
    virtual void OnUpdate() override;

private:
    /** Used for fast access to line render service. */
    std::shared_ptr<ILineRenderer> lineRenderer;

    /** The width of the output window. */
    int width;

    /** The height of the output window. */
    int height;
}; 