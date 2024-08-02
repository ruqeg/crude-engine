#include <directxmath/DirectXMath.h>
#include <flecs.h>
#include <SDL3/SDL.h>
#include <functional>

module crude.engine;

import crude.core.logger;
import crude.scene.free_camera_script;
import crude.platform.sdl_helper;
import crude.core.memory;

namespace crude
{

void Engine::initialize(const Engine_Initialize& config)
{
  core::initializeMemory(config.defaultFreeRBTCapacity);
  platform::initializeSDL();
  platform::loadSDLVulkan();
  initializeSystems();

  auto windowContainer = crude::core::allocateShared<crude::platform::SDL_Window_Container>(
    config.title, config.width, config.height, crude::platform::SDL_WINDOW_CONTAINER_FLAG_VULKAN);
  m_renderer = core::allocateShared<graphics::Renderer>(windowContainer, m_world);
  m_timer.setFrameRate(60);
}

void Engine::deinitialize()
{
  platform::deinitializeSDL();
  platform::unloadSDLVulkan();
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
  flecs::query<scene::script::Free_Camera_Component> freeCameraUpdateEvent = m_world.query_builder<scene::script::Free_Camera_Component>().build();
  m_world.set<platform::Input_System_Component>(platform::Input_System_Component([freeCameraUpdateEvent]() {
    freeCameraUpdateEvent.each(std::function(scene::script::freeCameraUpdateEventSystemProcess));
  }));

  m_inputSystem = m_world.system("InputSystem")
    .kind(flecs::PreUpdate)
    .run(platform::inputSystemProcess);

  m_freeCameraUpdateSystem = m_world.system<scene::script::Free_Camera_Component, scene::Transform>("FreeCameraUpdateSystem")
    .kind(flecs::OnUpdate)
    .each(std::function(scene::script::freeCameraUpdateSystemProcess));
}

void Engine::render()
{
  m_renderer->drawFrame();
}

}