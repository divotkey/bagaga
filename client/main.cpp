#include <iostream>

#include <ServiceManager.h>
#include <UpdateService.h>

#include <SdlService.h>
#include <SdlEventService.h>
#include <SdlVideoService.h>
#include "PhysicsService.h"

using namespace std;
using namespace astu;


int main()
{
  auto & sm = ServiceManager::GetInstance();
  sm.AddService(std::make_shared<UpdateService>());
  sm.AddService(std::make_shared<SdlService>(true));
  sm.AddService(std::make_shared<SdlVideoService>());
  sm.AddService(std::make_shared<SdlEventService>());
  sm.AddService(std::make_shared<PhysicsService>());

  // configure application
  sm.GetService<IWindowManager>().SetTitle("My Game");
  sm.GetService<IWindowManager>().SetSize(640, 480);

  // start game loop
  sm.StartupAll();

  auto & updater = sm.GetService<UpdateService>();
  auto & event = sm.GetService<SdlEventService>();

  while (!event.IsQuit()) {
    updater.UpdateAll();
  }

  sm.ShutdownAll();
    
  return 0;
}
