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

void Engine::initialize()
{
  m_inputSystemCtx = core::allocateShared<platform::Input_System_Context>();
}

void Engine::deinitialize()
{
  m_world.system().kind(0).run(graphics::rendererCoreComponentDeinitialize).run();
  m_world.system().kind(0).run(gui::imguiRendererPassSystemDeinitialize).run();
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
  m_world.set<core::shared_ptr<platform::SDL_Window_Container>>(crude::core::allocateShared<crude::platform::SDL_Window_Container>(
    initialize.title, initialize.width, initialize.height, crude::platform::SDL_WINDOW_CONTAINER_FLAG_VULKAN));
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
      .ctx(m_inputSystemCtx.get())
      .run(scripts::windowScriptInputSystemProcess),
    m_world.system("ImguiUpdateEvent")
      .kind(0)
      .ctx(m_inputSystemCtx.get())
      .run(gui::imguiInputSystemProcess)
  };
  inputSystems.insert(inputSystems.end(), initialize.inputSystems.begin(), initialize.inputSystems.end());
  m_inputSystemCtx->inputSystems = inputSystems;

  m_inputSystem = m_world.system("InputSystem")
    .ctx(m_inputSystemCtx.get())
    .kind(flecs::PreUpdate)
    .run(platform::inputSystemProcess);
}

void Engine::initializeRendererSystems(const Initialize_Systems& initialize)
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

  m_world.system().kind(0).run(graphics::rendererCoreComponentInitialize).run();
  m_world.system().kind(0).run(graphics::deferredGBufferPassSystemComponentInitialize).run();
  m_world.system().kind(0).run(graphics::fullscreenPBRPassComponentInitialize).run();
  m_world.system().kind(0).run(graphics::rendererFrameSystemComponentInitiailize).run();
  m_world.system().kind(0).run(gui::imguiRendererPassSystemInitialize).run();
}

}