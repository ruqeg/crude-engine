#include <flecs.h>

module application;

Application::Application()
{
  initialize(crude::Engine_Initialize {
    .defaultFreeRBTCapacity = 600000000,
    .width = 800,
    .height = 800,
    .title = "TEST",
  });
}

Application::~Application()
{
  deinitialize();
}

void Application::run()
{
  mainLoop();
}
