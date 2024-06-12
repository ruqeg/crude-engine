module application;

Application::Application(crude::core::shared_ptr<crude::system::SDL_Window_Container> windowContainer)
  :
  crude::Engine(windowContainer)
{}

void Application::run() noexcept
{
  mainLoop();
}
