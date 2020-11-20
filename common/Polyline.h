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

#include <EntityService.h>
#include <Vector2.h>
#include <Color.h>

class Polyline : public astu::EntityComponent {
public:
    using Polygon = std::vector<astu::Vector2>;

    astu::Color color;
    std::shared_ptr<Polygon> polygon;

    Polyline(std::shared_ptr<Polygon> poly, const astu::Color & c = astu::WebColors::Red)
        : color(c)
        , polygon(poly)
    {
        // Intentionally left empty.
    }
};