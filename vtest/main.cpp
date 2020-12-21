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

// Standard C++ Libryry
#include <iostream>
#include <string>

// AST Utilities
#include <AstUtils.h>
#include <ServiceManager.h>
#include <UpdateService.h>
#include <Mouse.h>
#include <SdlService.h>
#include <SdlEventService.h>
#include <SdlVideoService.h>
#include <SdlTimeService.h>
#include <StateService.h>
#include <EntityService.h>
#include <Events.h>

// Bagaga Commons
#include "SdlVulkanService.h"

// Applications specific

using namespace std;
using namespace astu;

const std::string kAppName = "Vulkan Test";
const std::string kAppVersion = "0.1.0";

/**
 * Adds services required for all application states.
 */
void AddCoreServices()
{
	// Fetch service manager (realized as a singleton)
	auto &sm = ServiceManager::GetInstance();

	// Add basic functionality.
	sm.AddService(std::make_shared<UpdateService>());
	sm.AddService(std::make_shared<StateService>());

	// Add services requried for SDL-based core functionality
	sm.AddService(std::make_shared<SdlService>(true, true));
	sm.AddService(std::make_shared<SdlVideoService>());
	sm.AddService(std::make_shared<SdlVulkanService>(true));
	sm.AddService(std::make_shared<SdlEventService>());
	sm.AddService(std::make_shared<SdlTimeService>());
	
	// Experimental event-based input handling.
	sm.AddService(std::make_shared<MouseButtonEventService>());
}

void AddApplicationStates()
{
	// Fetch central state service.
	auto & ss = ServiceManager::GetInstance().GetService<StateService>();

	// // Add line render Demo.
	// ss.CreateState("MovingLines"); // optional
	// ss.AddService("MovingLines", std::make_shared<WindowTitleService>("(MovingLines)"));
	// ss.AddService("MovingLines", std::make_shared<SdlLineRenderer>());
	// ss.AddService("MovingLines", std::make_shared<LineRendererTestService>());

	// // Add entity Demo.
	// ss.CreateState("Entities"); // optional
	// ss.AddService("Entities", std::make_shared<WindowTitleService>("(Entities)"));
	// ss.AddService("Entities", std::make_shared<EntityService>());
	// ss.AddService("Entities", std::make_shared<SdlLineRenderer>());
	// ss.AddService("Entities", std::make_shared<AutoRotateSystem>());
	// ss.AddService("Entities", std::make_shared<PolylineVisualSystem>());
	// ss.AddService("Entities", std::make_shared<EntityTestService>());

	// // Add blank screen.
	// ss.CreateState("Create Entities");	// optional
	// ss.AddService("Create Entities", std::make_shared<WindowTitleService>("(Create Entities)"));
	// ss.AddService("Create Entities", std::make_shared<EntityService>());
	// ss.AddService("Create Entities", std::make_shared<SdlLineRenderer>());
	// ss.AddService("Create Entities", std::make_shared<AutoRotateSystem>());
	// ss.AddService("Create Entities", std::make_shared<PolylineVisualSystem>());
	// ss.AddService("Create Entities", std::make_shared<CreateEntityTestService>());
}

int main()
{
	SayVersion();
	AddCoreServices();
	AddApplicationStates();

	// Fetch service manager (realized as a singleton)
	auto &sm = ServiceManager::GetInstance();

	// configure application
	sm.GetService<IWindowManager>().SetTitle(kAppName + " - Version " + kAppVersion);
	sm.GetService<IWindowManager>().SetSize(800, 600);
	sm.GetService<SdlVideoService>().EnableVulkanSupport(true);

	// Start services
	sm.StartupAll();

	// Run game loop
	auto &updater = sm.GetService<UpdateService>();
	auto &event = sm.GetService<SdlEventService>();

	while (!event.IsQuit())
	{
		updater.UpdateAll();
	}

	// Game loop has ended, shutdown services.
	sm.ShutdownAll();

	return 0;
}
