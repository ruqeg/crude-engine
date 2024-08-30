#include <DirectXMath.h>
#include <flecs.h>
#include <SDL3/SDL.h>
#include <vulkan/vulkan.h>
#include <functional>

module crude.engine;

import crude.core.logger;
import crude.core.memory;
import crude.scripts.window_script;
import crude.platform.sdl_helper;
import crude.platform.sdl_window_container;
import crude.gui.imgui_helper;
import crude.gui.imgui_input_system;
import crude.scene.mesh;

namespace crude
{

Renderer_System_Ctx::Renderer_System_Ctx()
{
  frameCtx.coreCtx                = &coreCtx;
  deferredGBufferPassCtx.frameCtx = &frameCtx;
  fullscreenPbrPassCtx.frameCtx   = &frameCtx;
  imguiPassCtx.frameCtx           = &frameCtx;
}

void Engine::preinitialize(core::uint32 defaultFreeRBTCapacity)
{
  core::initializeMemory(defaultFreeRBTCapacity);
  platform::initializeSDL();
  platform::loadSDLVulkan();
  gui::initializeImGuiContext();
}

void Engine::postdeinitialize()
{
  gui::deinitializeImGuiContext();
  platform::unloadSDLVulkan();
  platform::deinitializeSDL();
}

void Engine::initialize(const Initialize& initialize)
{
  initializeWindow(initialize.window);
  initializeSystems(initialize.systems);
}

void Engine::deinitialize()
{
  m_world.system()
    .ctx(&m_rendererSystemCtx.coreCtx)
    .kind(0)
    .run(graphics::rendererCoreSystemDeinitialize).run();
  m_world.system()
    .ctx(&m_rendererSystemCtx.imguiPassCtx)
    .kind(0)
    .run(gui::rendererImguiPassSystemDeinitialize).run();
}

void Engine::mainLoop()
{
  const auto windowScriptComponent = m_world.get<scripts::Window_Script_Component>();
  while (!windowScriptComponent->shouldClose)
  {
    m_world.progress();
  }
}

void Engine::initializeWindow(const Initialize_Window& initialize)
{
  m_windowContainer = crude::core::allocateShared<crude::platform::SDL_Window_Container>(
    initialize.title, initialize.width, initialize.height, crude::platform::SDL_WINDOW_CONTAINER_FLAG_VULKAN);
}

void Engine::initializeSystems(const Initialize_Systems& initialize)
{
  initializeInputSystems(initialize);
  initializeRendererSystems(initialize);
}

void Engine::initializeInputSystems(const Initialize_Systems& initialize)
{
  m_world.set<scripts::Window_Script_Component>({});

  core::vector<flecs::system> inputSystems = {
    m_world.system("WindowInputSystem")
      .kind(0)
      .ctx(&m_inputSystemCtx)
      .run(scripts::windowScriptInputSystemProcess),
    m_world.system("ImguiUpdateEvent")
      .kind(0)
      .ctx(&m_inputSystemCtx)
      .run(gui::imguiInputSystemProcess)
  };
  inputSystems.insert(inputSystems.end(), initialize.inputSystems.begin(), initialize.inputSystems.end());
  m_inputSystemCtx.inputSystems = inputSystems;

  m_inputSystem = m_world.system("InputSystem")
    .ctx(&m_inputSystemCtx)
    .kind(flecs::PreUpdate)
    .run(platform::inputSystemProcess);
}

void Engine::initializeRendererSystems(const Initialize_Systems& initialize)
{
  flecs::system deferredGBufferPassSystem = m_world.system<core::shared_ptr<graphics::Mesh_Buffer>, core::shared_ptr<scene::Mesh>>("DeferredGBufferPassSystem")
    .ctx(&m_rendererSystemCtx.deferredGBufferPassCtx)
    .kind(0)
    .run(graphics::rendererDeferredGBufferPassSystemProcess);

  flecs::system frameStartSystem = m_world.system("FrameStartSystem")
    .ctx(&m_rendererSystemCtx.frameCtx)
    .kind(0)
    .run(graphics::rendererFrameStartSystemProcess);

  flecs::system fullscreenPBRPassSystem = m_world.system("FullscreenPBRPassSystem")
    .ctx(&m_rendererSystemCtx.fullscreenPbrPassCtx)
    .kind(0)
    .run(graphics::rendererFullscreenPBRPassSystemProcess);

  flecs::system imguiPassSystem = m_world.system("ImguiPassSystem")
    .ctx(&m_rendererSystemCtx.imguiPassCtx)
    .kind(0)
    .run(gui::rendererImguiPassSystemProcess);

  flecs::system frameSubmitSystem = m_world.system("FrameSubmitSystem")
    .ctx(&m_rendererSystemCtx.frameCtx)
    .kind(0)
    .run(graphics::rendererFrameSubmitSystemProcess);

  m_rendererSystem = m_world.system("RendererFrameSubmitSystem")
    .kind(flecs::PreStore)
    .run([frameStartSystem, deferredGBufferPassSystem, fullscreenPBRPassSystem, imguiPassSystem, frameSubmitSystem](flecs::iter& it) {
      frameStartSystem.run();
      deferredGBufferPassSystem.run();
      fullscreenPBRPassSystem.run();
      imguiPassSystem.run();
      frameSubmitSystem.run();
    });

  m_rendererSystemCtx.coreCtx.windowContainer = m_windowContainer;
  m_world.system()
    .ctx(&m_rendererSystemCtx.coreCtx)
    .kind(0)
    .run(graphics::rendererCoreSystemInitialize).run();
  m_world.system()
    .ctx(&m_rendererSystemCtx.deferredGBufferPassCtx)
    .kind(0)
    .run(graphics::rendererDeferredGBufferPassSystemInitialize).run();
  m_rendererSystemCtx.fullscreenPbrPassCtx.gbuffer = m_rendererSystemCtx.deferredGBufferPassCtx.gbuffer;
  m_world.system()
    .ctx(&m_rendererSystemCtx.fullscreenPbrPassCtx)
    .kind(0)
    .run(graphics::rendererFullscreenPBRPassSystemInitialize).run();
  m_world.system()
    .ctx(&m_rendererSystemCtx.frameCtx)
    .kind(0)
    .run(graphics::rendererFrameSystemInitiailize)
    .run();
  m_rendererSystemCtx.imguiPassCtx.layoutsDrawSystems = initialize.imguiLayoutSystems;
  m_world.system()
    .ctx(&m_rendererSystemCtx.imguiPassCtx)
    .kind(0)
    .run(gui::rendererImguiPassSystemInitialize)
    .run();
}

}