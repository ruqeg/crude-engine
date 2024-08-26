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
import crude.platform.input_system;
import crude.graphics.renderer_core_component;
import crude.graphics.deferred_gbuffer_pass_system;
import crude.graphics.fullscreen_pbr_pass_system;
import crude.graphics.renderer_frame_system;
import crude.gui.imgui_helper;
import crude.gui.imgui_renderer_pass_system;
import crude.gui.imgui_input_system;
import crude.scene.mesh;

namespace crude
{

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
  initializeInputSystem(initialize.systems);
  initializeRendererSystem(initialize.systems);
  initializeRendererComponents();
}

void Engine::deinitialize()
{
  m_world.system().kind(0).run(graphics::rendererCoreComponentDeinitialize).run();
  m_world.system().kind(0).run(gui::imguiRendererPassSystemDeinitialize).run();
}

void Engine::mainLoop()
{
  const auto windowComponent = m_world.get<scripts::Window_Component>();
  while (!windowComponent->shouldClose)
  {
    m_world.progress();
  }
}

void Engine::initializeWindow(const Initialize_Window& initialize)
{
  m_world.set<core::shared_ptr<platform::SDL_Window_Container>>(crude::core::allocateShared<crude::platform::SDL_Window_Container>(
    initialize.title, initialize.width, initialize.height, crude::platform::SDL_WINDOW_CONTAINER_FLAG_VULKAN));
}

void Engine::initializeInputSystem(const Initialize_Systems& initialize)
{
  m_world.set<scripts::Window_Component>({});

  core::vector<flecs::system> inputSystems = {
   m_world.system("WindowUpdateEvent").kind(0)
     .run(scripts::windowUpdateEventSystemProcess),
   m_world.system("ImguiUpdateEvent").kind(0)
     .run(gui::imguiUpdateEventSystemProcess)
  };
  inputSystems.insert(inputSystems.end(), initialize.inputSystems.begin(), initialize.inputSystems.end());

  m_world.set<platform::Input_System_Component>(platform::Input_System_Component(inputSystems));
  
  m_inputSystem = m_world.system("InputSystem")
    .kind(flecs::PreUpdate)
    .run(platform::inputSystemProcess);
}

void Engine::initializeRendererSystem(const Initialize_Systems& initialize)
{
  m_world.set<gui::ImGui_Layout_Component>(gui::ImGui_Layout_Component(initialize.imguiLayoutSystems));

  flecs::system deferredGBufferPassSystem = m_world.system<core::shared_ptr<graphics::Mesh_Buffer>, core::shared_ptr<scene::Mesh>>("DeferredGBufferPassSystem").kind(0)
    .run(graphics::deferredGBufferPassSystemProcess);

 m_rendererSystem = m_world.system("RendererFrameSubmitSystem")
    .kind(flecs::PreStore)
    .run([deferredGBufferPassSystem](flecs::iter& it) {
      graphics::rendererFrameStartSystemProcess(it);
      deferredGBufferPassSystem.run();
      graphics::fullscreenPBRPassSystemProcess(it);
      gui::imguiRendererPassSystemProcess(it);
      graphics::rendererFrameSubmitSystemProcess(it);
    });
}

void Engine::initializeRendererComponents()
{
  m_world.system().kind(0).run(graphics::rendererCoreComponentInitialize).run();
  m_world.system().kind(0).run(graphics::deferredGBufferPassSystemComponentInitialize).run();
  m_world.system().kind(0).run(graphics::fullscreenPBRPassComponentInitialize).run();
  m_world.system().kind(0).run(graphics::rendererFrameSystemComponentInitiailize).run();
  m_world.system().kind(0).run(gui::imguiRendererPassSystemInitialize).run();
}

}