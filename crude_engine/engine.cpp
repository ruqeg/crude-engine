module crude.engine;

import crude.core.logger;

namespace crude
{

Engine::Engine(core::shared_ptr<system::SDL_Window_Container> windowContainer)
  :
  m_renderer(windowContainer)
{
  m_timer.setFrameRate(60);
  m_renderer.getCamera().setPosition(0.0, 0.0, -5.0);
}

void Engine::mainLoop()
{
  bool quit = false;
  while (quit == false)
  {
    m_ioManager.update();
    if (m_ioManager.getWindowEH().readEvent().shouldClose())
      quit = true;

    core::float64 elapsed;
    if (m_timer.frameElasped(elapsed))
    {
      update(elapsed);
      render();
    }
  }
}

void Engine::update(core::float64 elapsed)
{
  // !TODO :D
  if (m_ioManager.getKeyboardEH().keyIsPressed(system::Scancode::SDL_SCANCODE_W))
  {
    m_renderer.getCamera().addPosition(math::svector::scale(m_renderer.getCamera().getForwardVector(), 7 * elapsed));
  }
  if (m_ioManager.getKeyboardEH().keyIsPressed(system::Scancode::SDL_SCANCODE_S))
  {
    m_renderer.getCamera().addPosition(math::svector::scale(m_renderer.getCamera().getForwardVector(), -7 * elapsed));
  }
  if (m_ioManager.getKeyboardEH().keyIsPressed(system::Scancode::SDL_SCANCODE_A))
  {
    m_renderer.getCamera().addPosition(math::svector::scale(m_renderer.getCamera().getRightVector(), -7 * elapsed));
  }
  if (m_ioManager.getKeyboardEH().keyIsPressed(system::Scancode::SDL_SCANCODE_D))
  {
    m_renderer.getCamera().addPosition(math::svector::scale(m_renderer.getCamera().getRightVector(), 7 * elapsed));
  }
  if (m_ioManager.getKeyboardEH().keyIsPressed(system::Scancode::SDL_SCANCODE_E))
  {
    m_renderer.getCamera().addPosition(math::svector::scale(m_renderer.getCamera().getTopVector(), -7 * elapsed));
  }
  if (m_ioManager.getKeyboardEH().keyIsPressed(system::Scancode::SDL_SCANCODE_Q))
  {
    m_renderer.getCamera().addPosition(math::svector::scale(m_renderer.getCamera().getTopVector(), 7 * elapsed));
  }
  while (!m_ioManager.getMouseEH().eventBufferIsEmpty())
  {
    system::Mouse_Event me = m_ioManager.getMouseEH().readEvent();
    if (me.getType() == system::MOUSE_EVENT_TYPE_MOVE)
    {
      if (m_ioManager.getMouseEH().isRightDown())
      {
        m_renderer.getCamera().addRotation(
          -0.15 * me.getPositionRelY() * elapsed,
          0.15 * me.getPositionRelX() * elapsed,
          0.f);
      }
    }
  }
}

void Engine::render()
{
  m_renderer.drawFrame();
}

void Engine::initialize(const Engine_Config& config)
{
  initializeMemory(config.defaultFreeRBTCapacity);
  initalizeSystem();
  initalizeNetwork();
}

void Engine::initializeMemory(core::uint32 defaultFreeRBTCapacity)
{
  core::Memory_Manager::getInstance().initialize(defaultFreeRBTCapacity);
}

void Engine::initalizeSystem()
{
  crude::system::SDL_System::getInstance().initialize();
  crude::system::SDL_System::getInstance().initializeVulkan();
}

void Engine::initalizeNetwork()
{
  crude::network::Network_System::getInstance().initialize();
}

}