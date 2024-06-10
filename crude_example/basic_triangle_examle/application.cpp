module application;

Application::Application(crude::core::Shared_Ptr<crude::system::SDL_Window_Container> windowContainer)
  :
  crude::Engine(windowContainer)
{}

void Application::run() noexcept
{
  mainLoop();
}
