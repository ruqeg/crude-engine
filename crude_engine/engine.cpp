#include <directxmath/DirectXMath.h>
#include <flecs.h>
#include <SDL3/SDL.h>
#include <functional>

module crude.engine;

import crude.core.logger;
import crude.scene.free_camera_script;
import crude.scene.window_script;
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
  auto rendererBase = core::allocateShared<graphics::Renderer_Base>(windowContainer);
  m_renderer = core::allocateShared<graphics::Renderer>(rendererBase, m_world);
  m_timer.setFrameRate(60);
}

void Engine::deinitialize()
{
  platform::deinitializeSDL();
  platform::unloadSDLVulkan();
}

void Engine::mainLoop()
{
  const auto windowComponent = m_world.get<scene::script::Window_Component>();
  while (!windowComponent->shouldClose)
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
  m_world.set<scene::script::Window_Component>({});

  flecs::query<scene::script::Free_Camera_Component> freeCameraUpdateEvent = m_world.query_builder<scene::script::Free_Camera_Component>().build();
  flecs::query windowUpdateEvent = m_world.query_builder().build();
  m_world.set<platform::Input_System_Component>(platform::Input_System_Component([freeCameraUpdateEvent, windowUpdateEvent]() {
    freeCameraUpdateEvent.each(std::function(scene::script::freeCameraUpdateEventSystemEach));
    windowUpdateEvent.run(std::function(scene::script::windowUpdateEventSystemProcess));
  }));

  m_inputSystem = m_world.system("InputSystem")
    .kind(flecs::PreUpdate)
    .run(platform::inputSystemProcess);

  m_freeCameraUpdateSystem = m_world.system<scene::script::Free_Camera_Component, scene::Transform>("FreeCameraUpdateSystem")
    .kind(flecs::OnUpdate)
    .each(std::function(scene::script::freeCameraUpdateSystemEach));
}

void Engine::render()
{
  m_renderer->drawFrame();
}

}