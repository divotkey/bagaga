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

#include <IWindowManager.h>
#include "WindowTitleService.h"

using namespace astu;

WindowTitleService::WindowTitleService(const std::string & _title, bool restore, bool replace)
    : title(_title)
    , restoreOrig(restore)
    , replaceOrig(replace)
{
    // Intentionally left empty.
}

void WindowTitleService::OnStartup()
{
    auto & wm = GetSM().GetService<IWindowManager>();
    origTitle = wm.GetTitle();
    if (replaceOrig) {
        wm.SetTitle(title);
    } else {
        wm.SetTitle(origTitle + " " + title);
    }
}

void WindowTitleService::OnShutdown()
{
    if (restoreOrig) {
        GetSM().GetService<IWindowManager>().SetTitle(origTitle);
    }
}

