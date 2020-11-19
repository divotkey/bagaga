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
#include <string>

#include <ServiceManager.h>
#include <UpdateService.h>

#include <SdlService.h>
#include <SdlEventService.h>
#include <SdlVideoService.h>
#include <SdlRenderService.h>
#include <SdlTimeService.h>
#include <SdlAudioService.h>

#include <EntityService.h>

#include "SdlLineRenderer.h"
#include "Pose2D.h"
#include "CommandQueue.h"
#include "ListenerManager.h"
#include "SignalService.h"

using namespace std;
using namespace astu;

const std::string kAppName = "Bagaga Client";
const std::string kAppVersion = "0.1.0";

int main()
{
	auto &sm = ServiceManager::GetInstance();
	sm.AddService(std::make_shared<UpdateService>());

	sm.AddService(std::make_shared<SdlService>(true));
	sm.AddService(std::make_shared<SdlVideoService>());
	sm.AddService(std::make_shared<SdlEventService>());
	sm.AddService(std::make_shared<SdlRenderService>());
	sm.AddService(std::make_shared<SdlTimeService>());

	sm.AddService(std::make_shared<SdlLineRenderer>(0));

	// configure application
	sm.GetService<IWindowManager>().SetTitle(kAppName + " - Version " + kAppVersion);
	sm.GetService<IWindowManager>().SetSize(640, 480);

	// start game loop
	sm.StartupAll();

	auto &updater = sm.GetService<UpdateService>();
	auto &event = sm.GetService<SdlEventService>();

	while (!event.IsQuit())
	{
		updater.UpdateAll();
	}

	sm.ShutdownAll();

	return 0;
}
