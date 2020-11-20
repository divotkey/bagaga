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

#include <string>
#include <Service.h>

/**
 * Small utility service which changes the window title on startup.
 * This service requires a service which is implementing the 
 * IWindowManager interface.
 */
class WindowTitleService : public astu::BaseService {
public:

    /**
     * Constructor.
     * 
     * @param title         the window title to set on startup
     * @param restoreOrig   whether to restore the original title on shutdown
     * @param replace       whether to add the title to the original title or replace
     */
    WindowTitleService(const std::string & title, bool restoreOrig = true, bool replace = false);
    
protected:

    // Inherited via BaseService
    virtual void OnStartup() override;
    virtual void OnShutdown() override;

private:
    /** The window title to set. */
    std::string title;

    /** The orig window title. */
    std::string origTitle;

    /** Whether to restore the previous title on shutdown. */    
    bool restoreOrig;

    /** Whether to replace the original title. */
    bool replaceOrig;
};