#include <directxmath/DirectXMath.h>

module crude.engine;

import crude.core.logger;
import crude.resources.gltf_loader;

namespace crude
{

Engine::Engine(core::shared_ptr<system::SDL_Window_Container> windowContainer)
{
  core::shared_ptr<scene::Camera> camera = core::allocateShared<scene::Camera>();
  camera->calculateViewToClipMatrix(DirectX::XM_PIDIV4, windowContainer->getAspect(), 0.1f, 10.0f);
  camera->setPosition(0, 0, -2);
  resources::GLTF_Loader gltfLoader;
  m_world = gltfLoader.loadWorldFromFile("../../crude_example/basic_triangle_examle/resources/helmet.glb").value();
  m_world->setCamera(camera);
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
  core::shared_ptr<scene::Camera> camera = m_world->getCamera();
  // !TODO :D
  if (m_ioManager.getKeyboardEH().keyIsPressed(system::Scancode::SDL_SCANCODE_W))
  {
    camera->addPosition(DirectX::XMVectorScale(camera->getForwardVector(), 7 * elapsed));
  }
  if (m_ioManager.getKeyboardEH().keyIsPressed(system::Scancode::SDL_SCANCODE_S))
  {
    camera->addPosition(DirectX::XMVectorScale(camera->getForwardVector(), -7 * elapsed));
  }
  if (m_ioManager.getKeyboardEH().keyIsPressed(system::Scancode::SDL_SCANCODE_A))
  {
    camera->addPosition(DirectX::XMVectorScale(camera->getRightVector(), -7 * elapsed));
  }
  if (m_ioManager.getKeyboardEH().keyIsPressed(system::Scancode::SDL_SCANCODE_D))
  {
    camera->addPosition(DirectX::XMVectorScale(camera->getRightVector(), 7 * elapsed));
  }
  if (m_ioManager.getKeyboardEH().keyIsPressed(system::Scancode::SDL_SCANCODE_E))
  {
    camera->addPosition(DirectX::XMVectorScale(camera->getTopVector(), -7 * elapsed));
  }
  if (m_ioManager.getKeyboardEH().keyIsPressed(system::Scancode::SDL_SCANCODE_Q))
  {
    camera->addPosition(DirectX::XMVectorScale(camera->getTopVector(), 7 * elapsed));
  }
  while (!m_ioManager.getMouseEH().eventBufferIsEmpty())
  {
    system::Mouse_Event me = m_ioManager.getMouseEH().readEvent();
    if (me.getType() == system::MOUSE_EVENT_TYPE_MOVE)
    {
      if (m_ioManager.getMouseEH().isRightDown())
      {
        camera->addRotation(
          -0.15 * me.getPositionRelY() * elapsed,
          0.15 * me.getPositionRelX() * elapsed,
          0.f);
      }
    }
  }
  camera->calculateWorldToViewMatrix();
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