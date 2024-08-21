#include <flecs.h>

module application;

Application::Application()
{
  initialize("TEST", 800, 800);
}

void Application::run()
{
  mainLoop();
}
