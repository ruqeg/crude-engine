module application;

Application::Application(crude::core::Shared_Ptr<crude::system::SDL_Window_Container> windowContainer)
  :
  crude::Engine(windowContainer)
{}

void Application::run() noexcept
{
  mainLoop();
}

void Application::mainLoop()
{
  bool quit = false;
  while (quit == false)
  {
    m_ioManager.update();
    if (m_ioManager.getWindowEH().readEvent().shouldClose())
      quit = true;
    m_renderer.drawFrame();
  }

}
