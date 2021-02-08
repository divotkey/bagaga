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

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <IWindowManager.h>
#include <ServiceManager.h>
#include <AstUtils.h>
#include "LineRendererTestService.h"

#define MAX_VEL     550.0
#define MIN_VEL     150.0
#define NUM_LINES   500

LineRendererTestService::LineRendererTestService(bool _drawStatic)
    : drawStatic(_drawStatic)
{
    // Intentionally left empty.
}

void LineRendererTestService::OnStartup() 
{
    // Used to have fast access to line renderer.
    lineRenderer = GetSM().FindService<ILineRenderer>();
    if (!lineRenderer) {
        throw std::logic_error("Line renderer required");
    }

    timeService = GetSM().FindService<astu::ITimeService>();
    if (!timeService) {
        throw std::logic_error("Time service required");
    }

    auto & wm = GetSM().GetService<astu::IWindowManager>();
    width = wm.GetWidth();
    height = wm.GetHeight();

    for (int i = 0; i <NUM_LINES; ++i) {
        lines.push_back(MovingLine(width, height));
    }
}

void LineRendererTestService::OnShutdown() 
{
    // Cleanup.
    lineRenderer = nullptr;
    timeService = nullptr;
}

void LineRendererTestService::OnUpdate() 
{
    // Draw static cross.
    if (drawStatic) {
        lineRenderer->SetDrawColor(astu::WebColors::Red);    
        lineRenderer->DrawLine(0, 0, width, height);
        lineRenderer->SetDrawColor(astu::WebColors::Green);
        lineRenderer->DrawLine(width, 0, 0, height);
        lineRenderer->SetDrawColor(astu::WebColors::Blue);
        lineRenderer->DrawLine(width / 2, 0, width / 2, height);
        lineRenderer->SetDrawColor(astu::WebColors::Yellow);
        lineRenderer->DrawLine(0, height / 2, width, height / 2);
    }

    // Update moving lines
    for (auto & line : lines) {
        UpdateLine(line, timeService->GetElapsedTime());
    }

    // Render moving line.
    lineRenderer->SetDrawColor(astu::WebColors::White);
    for (auto & line : lines) {
        RenderLine(line);
    }
}

LineRendererTestService::MovingLine::MovingLine(double width, double height)
{
    p1.Set(GetRandomDouble(0, width), GetRandomDouble(0, height));
    p2.Set(GetRandomDouble(0, width), GetRandomDouble(0, height));

    v1.Set(GetRandomDouble(MIN_VEL, MAX_VEL), 0);
    v1.Rotate(GetRandomDouble(0, 2 * M_PI));

    v2.Set(GetRandomDouble(MIN_VEL, MAX_VEL), 0);
    v2.Rotate(GetRandomDouble(0, 2 * M_PI));
}

void LineRendererTestService::UpdateLine(MovingLine& line, double dt)
{
    line.p1 += line.v1 * dt;
    line.p2 += line.v2 * dt;
    
    KeepWithinBoundaries(line.p1, line.v1);
    KeepWithinBoundaries(line.p2, line.v2);
}

void LineRendererTestService::RenderLine(MovingLine& line)
{
    lineRenderer->DrawLine(line.p1, line.p2);
}

void LineRendererTestService::KeepWithinBoundaries(astu::Vector2<double> & p, astu::Vector2<double> & v)
{
    if (p.x < 0) {
        p.x = 0;
        v.x = -v.x;
    }
    if (p.x >= width) {
        p.x = width - 1;
        v.x = -v.x;
    }

    if (p.y < 0)  {
        p.y = 0;
        v.y = -v.y;
    }

    if (p.y >= height) {
        p.y = height - 1;
        v.y = -v.y;
    }
}

