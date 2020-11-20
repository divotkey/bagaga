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
#include <ServiceManager.h>
#include <UpdateService.h>
#include <Mouse.h>
#include <SdlService.h>
#include <SdlEventService.h>
#include <SdlVideoService.h>
#include <SdlRenderService.h>
#include <SdlTimeService.h>
#include <SdlAudioService.h>
#include <StateService.h>
#include <EntityService.h>
#include <SignalService.h>
#include <Events.h>

// Bagaga Commons
#include "SdlLineRenderer.h"
#include "WindowTitleService.h"

// Applications specific
#include "LineRendererTestService.h"


using namespace std;
using namespace astu;

const std::string kAppName = "Bagaga Demo";
const std::string kAppVersion = "0.2.0";

class MyButtonHandler : public astu::MouseButtonListener {
public:

	MyButtonHandler()
		: stateIdx(0)
	{
		// Intentionally left empty.
	}

	virtual void OnSignal(const MouseButtonEvent & event) override {
		if (!event.pressed || event.button != MouseButtonEvent::LEFT) {
			// Only process button down events.
			return;
		}
		// std::cout << "button down " << event.button << std::endl;

		stateIdx = (stateIdx + 1) % 2;
		SwitchState();
	}

private:

	int stateIdx;

	void SwitchState() {
		auto & sm = ServiceManager::GetInstance().GetService<StateService>();

		// Hardcoded simple state machine.
		switch (stateIdx) {
		case 0:
			ServiceManager::GetInstance().GetService<StateService>()
				.SwitchState("MovingLines");
			break;
		case 1:
			ServiceManager::GetInstance().GetService<StateService>()
				.SwitchState("Blank");
			break;

		default:
			// do nothing.
			break;
		}
	}
};

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
	sm.AddService(std::make_shared<SdlService>(true));
	sm.AddService(std::make_shared<SdlVideoService>());
	sm.AddService(std::make_shared<SdlEventService>());
	sm.AddService(std::make_shared<SdlRenderService>());
	sm.AddService(std::make_shared<SdlTimeService>());
	
	// Experimental event-based input handling.
	sm.AddService(std::make_shared<MouseButtonEventService>());
	sm.GetService<MouseButtonEventService>().AddListener(std::make_shared<MyButtonHandler>());
}

void AddApplicationStates()
{
	// Fetch central state service.
	auto & ss = ServiceManager::GetInstance().GetService<StateService>();

	// Add line render Demo.
	ss.CreateState("MovingLines"); // optional
	ss.AddService("MovingLines", std::make_shared<WindowTitleService>("(MovingLines)"));
	ss.AddService("MovingLines", std::make_shared<SdlLineRenderer>(0));
	ss.AddService("MovingLines", std::make_shared<LineRendererTestService>());

	// Add blank screen.
	ss.CreateState("Blank");	// optional
	ss.AddService("Blank", std::make_shared<WindowTitleService>("(Blank)"));
}

int main()
{
	AddCoreServices();
	AddApplicationStates();

	Mouse mouse;

	if (mouse.IsPressed(1)) {
		std::cout << "mouse button pressed" << std::endl;
	}

	// Fetch service manager (realized as a singleton)
	auto &sm = ServiceManager::GetInstance();

	// configure application
	sm.GetService<IWindowManager>().SetTitle(kAppName + " - Version " + kAppVersion);
	sm.GetService<IWindowManager>().SetSize(640, 480);

	// Start services
	sm.StartupAll();

	sm.GetService<StateService>().SwitchState("MovingLines");

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
