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

class LinearMovement : public astu::EntityComponent {
public:
    /** The velocity of the linear movement. */
    astu::Vector2<double> vel;

    /**
     * Constructor.
     * 
     * @param v the velicoty
     */
    LinearMovement(const astu::Vector2<double> & v)
        : vel(v)
    {
        // Intentionally left empty.
    }

    /**
     * Constructor.
     * 
     * @param vx the x-component of the velicoty
     * @param vy the y-component of the velicoty
     */
    LinearMovement(double vx, double vy)
        : vel(vx, vy)
    {
        // Intentionally left empty.
    }

};