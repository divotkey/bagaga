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

#include <iostream>
#include <IWindowManager.h>
#include <ServiceManager.h>
#include "LineRendererTestService.h"

void LineRendererTestService::OnStartup() 
{
    // Used to have fast access to line renderer.
    lineRenderer = GetSM().FindService<ILineRenderer>();
    if (!lineRenderer) {
        throw std::logic_error("Line renderer required");
    }

    auto & wm = GetSM().GetService<astu::IWindowManager>();
    width = wm.GetWidth();
    height = wm.GetHeight();
}

void LineRendererTestService::OnShutdown() 
{
    // Cleanup.
    lineRenderer = nullptr;
}

void LineRendererTestService::OnUpdate() 
{
    lineRenderer->SetDrawColor(astu::WebColors::Red);
    lineRenderer->DrawLine(0, 0, width, height);
    lineRenderer->SetDrawColor(astu::WebColors::Green);
    lineRenderer->DrawLine(width, 0, 0, height);
    lineRenderer->SetDrawColor(astu::WebColors::Blue);
    lineRenderer->DrawLine(width / 2, 0, width / 2, height);
    lineRenderer->SetDrawColor(astu::WebColors::Yellow);
    lineRenderer->DrawLine(0, height / 2, width, height / 2);
}
