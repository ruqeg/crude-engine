#include <directxmath/DirectXMath.h>
#include <flecs.h>
#include <SDL3/SDL.h>
#include <functional>

module crude.engine;

import crude.core.logger;
import crude.scene.free_camera;
import crude.system.sdl_helper;
import crude.core.memory;

namespace crude
{

void Engine::initialize(const Engine_Initialize& config)
{
  core::initializeMemory(config.defaultFreeRBTCapacity);
  system::initializeSDL();
  system::loadSDLVulkan();
  initializeSystems();

  auto windowContainer = crude::core::allocateShared<crude::system::SDL_Window_Container>(
    config.title, config.width, config.height, crude::system::SDL_WINDOW_CONTAINER_FLAG_VULKAN);
  m_renderer = core::allocateShared<graphics::Renderer>(windowContainer, m_world);
  m_timer.setFrameRate(60);
}

void Engine::deinitialize()
{
  system::deinitializeSDL();
  system::unloadSDLVulkan();
}

void Engine::mainLoop()
{
  while (m_quit == false)
  {
    m_inputSystem.run();

    core::float64 elapsed;
    if (m_timer.frameElasped(elapsed))
    {
      core::logInfo(core::Debug::Channel::All, "fps: %i\n", (int)(1.0 / elapsed));
      m_freeCameraUpdateSystem.run(elapsed);
      render();
    }
  }
}

void Engine::initializeSystems()
{
  flecs::query<scene::Free_Camera_Component> freeCameraUpdateEvent = m_world.query_builder<scene::Free_Camera_Component>().build();
  m_world.set<system::Input_System_Component>(system::Input_System_Component([freeCameraUpdateEvent]() {
    freeCameraUpdateEvent.each(std::function(scene::freeCameraUpdateEventSystemProcess));
  }));

  m_inputSystem = m_world.system("InputSystem")
    .kind(flecs::PreUpdate)
    .run(system::inputSystemProcess);

  m_freeCameraUpdateSystem = m_world.system<scene::Free_Camera_Component, scene::Transform>("FreeCameraUpdateSystem")
    .kind(flecs::OnUpdate)
    .each(std::function(scene::freeCameraUpdateSystemProcess));
}

void Engine::render()
{
  m_renderer->drawFrame();
}

}