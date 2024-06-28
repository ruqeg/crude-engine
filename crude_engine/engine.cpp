module crude.engine;

import crude.core.logger;
import crude.math.constants;
import crude.resources.world_loader;

namespace crude
{

Engine::Engine(core::shared_ptr<system::SDL_Window_Container> windowContainer)
{
  scene::Camera camera;
  camera.calculateViewToClipMatrix(math::CPI4, windowContainer->getAspect(), 0.1f, 10.0f);
  camera.setPosition(0, 0, -2);

  //m_world = resources::loadWorld("../../crude_example/basic_triangle_examle/resources/box_textured.glb").value();
  m_world = core::allocateShared<scene::World>(core::vector{ camera });
  m_renderer = core::allocateShared<graphics::Renderer>(windowContainer, m_world);
  m_timer.setFrameRate(60);
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
      core::logInfo(core::Debug::Channel::All, "fps: %i\n", (int)(1.0 / elapsed));
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
    m_world->getCamera().addPosition(math::svector::scale(m_world->getCamera().getForwardVector(), 7 * elapsed));
  }
  if (m_ioManager.getKeyboardEH().keyIsPressed(system::Scancode::SDL_SCANCODE_S))
  {
    m_world->getCamera().addPosition(math::svector::scale(m_world->getCamera().getForwardVector(), -7 * elapsed));
  }
  if (m_ioManager.getKeyboardEH().keyIsPressed(system::Scancode::SDL_SCANCODE_A))
  {
    m_world->getCamera().addPosition(math::svector::scale(m_world->getCamera().getRightVector(), -7 * elapsed));
  }
  if (m_ioManager.getKeyboardEH().keyIsPressed(system::Scancode::SDL_SCANCODE_D))
  {
    m_world->getCamera().addPosition(math::svector::scale(m_world->getCamera().getRightVector(), 7 * elapsed));
  }
  if (m_ioManager.getKeyboardEH().keyIsPressed(system::Scancode::SDL_SCANCODE_E))
  {
    m_world->getCamera().addPosition(math::svector::scale(m_world->getCamera().getTopVector(), -7 * elapsed));
  }
  if (m_ioManager.getKeyboardEH().keyIsPressed(system::Scancode::SDL_SCANCODE_Q))
  {
    m_world->getCamera().addPosition(math::svector::scale(m_world->getCamera().getTopVector(), 7 * elapsed));
  }
  while (!m_ioManager.getMouseEH().eventBufferIsEmpty())
  {
    system::Mouse_Event me = m_ioManager.getMouseEH().readEvent();
    if (me.getType() == system::MOUSE_EVENT_TYPE_MOVE)
    {
      if (m_ioManager.getMouseEH().isRightDown())
      {
        m_world->getCamera().addRotation(
          -0.15 * me.getPositionRelY() * elapsed,
          0.15 * me.getPositionRelX() * elapsed,
          0.f);
      }
    }
  }
  m_world->getCamera().calculateWorldToViewMatrix();
}

void Engine::render()
{
  m_renderer->drawFrame();
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