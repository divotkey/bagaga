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

#include <cassert>
#include <SDL2/SDL.h>
#include "SdlLineRenderer.h"


SdlLineRenderer::SdlLineRenderer(int renderPriority)
    : astu::BaseSdlRenderLayer(renderPriority, "SDL Line Renderer")
{
    // Intenitonally left empty.
}

void SdlLineRenderer::OnRender(SDL_Renderer* renderer)
{
    for (auto const & cmd : commands) {
        switch (cmd.type) {
        case CommandType::DRAW_LINE:
            SDL_RenderDrawLine(renderer, cmd.line.x1, cmd.line.y1, cmd.line.x2, cmd.line.y2);
            break;

        case CommandType::SET_COLOR:
            SDL_SetRenderDrawColor(
                renderer,
                static_cast<uint8_t>(cmd.color.r), 
                static_cast<uint8_t>(cmd.color.g), 
                static_cast<uint8_t>(cmd.color.b), 
                static_cast<uint8_t>(cmd.color.a)
                );
            break;
        }
    }
    commands.clear();
}

void SdlLineRenderer::DrawLine(double x1, double y1, double x2, double y2)
{

    RenderCommand cmd;

    cmd.type = CommandType::DRAW_LINE;
    cmd.line.x1 = x1;
    cmd.line.y1 = y1;
    cmd.line.x2 = x2;
    cmd.line.y2 = y2;

    commands.push_back(cmd);
}

void SdlLineRenderer::SetDrawColor(const astu::Color & c) 
{
    assert(c.r >= 0 && c.r <= 1);
    assert(c.g >= 0 && c.g <= 1);
    assert(c.b >= 0 && c.b <= 1);
    assert(c.a >= 0 && c.a <= 1);

    RenderCommand cmd;
    cmd.type = CommandType::SET_COLOR;
    cmd.color.r = c.r * 255;
    cmd.color.g = c.g * 255;
    cmd.color.b = c.b * 255;
    cmd.color.a = c.a * 255;

    commands.push_back(cmd);
}


void SdlLineRenderer::OnStartup()
{
    // Intentionally left empty.
}

void SdlLineRenderer::OnShutdown()
{
    commands.clear();
    commands.resize(0);
    commands.shrink_to_fit();
}