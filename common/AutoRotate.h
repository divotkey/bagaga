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

class AutoRotate : public astu::EntityComponent {
public:
    double speed;

    /**
     * Constructor.
     * 
     * @param s the rotation speed in radians per second
     */
    AutoRotate(double s = 1.0)
        : speed(s)
    {
        // Intentionally left empty.
    }
};