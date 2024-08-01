#include <directxmath/DirectXMath.h>
#include <flecs.h>
#include <SDL3/SDL.h>
#include <functional>

module crude.engine;

import crude.core.logger;
import crude.scene.free_camera;
import crude.system.sdl_helper;

namespace crude
{

void Engine::initialize(const Engine_Initialize& config)
{
  initializeMemory(config.defaultFreeRBTCapacity);
  initalizeSDL();

  auto windowContainer = crude::core::allocateShared<crude::system::SDL_Window_Container>(
    config.title, config.width, config.height, crude::system::SDL_WINDOW_CONTAINER_FLAG_VULKAN);
  m_renderer = core::allocateShared<graphics::Renderer>(windowContainer, m_world);
  m_timer.setFrameRate(60);
}

void Engine::deinitialize()
{
  deinitalizeSDL();
}

void Engine::mainLoop()
{
  while (m_quit == false)
  {
    updateEvent();

    core::float64 elapsed;
    if (m_timer.frameElasped(elapsed))
    {
      core::logInfo(core::Debug::Channel::All, "fps: %i\n", (int)(1.0 / elapsed));
      update(elapsed);
      render();
    }
  }
}

void Engine::updateEvent()
{
  SDL_Event inputEvent;
  while (SDL_PollEvent(&inputEvent))
  {
    if (inputEvent.type == SDL_EVENT_QUIT)
    {
      m_quit = true;
      return;
    }

    auto q = m_world.query<scene::Free_Camera_Component>();
    m_world.set<SDL_Event>(inputEvent);
    q.each(std::function(scene::freeCameraUpdateEvent));
  }
}

void Engine::update(core::float64 elapsed)
{
  auto q = m_world.query<scene::Free_Camera_Component, scene::Transform>();
  m_world.set<core::float64>(elapsed);
  q.each(std::function(scene::freeCameraUpdate));
}

void Engine::render()
{
  m_renderer->drawFrame();
}

void Engine::initializeMemory(core::uint32 defaultFreeRBTCapacity)
{
  core::Memory_Manager::getInstance().initialize(defaultFreeRBTCapacity);
}

void Engine::initalizeSDL()
{
  system::initializeSDL();
  system::loadSDLVulkan();
}

void Engine::deinitalizeSDL()
{
  system::deinitializeSDL();
  system::unloadSDLVulkan();
}

}