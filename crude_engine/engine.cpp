#include <DirectXMath.h>
#include <flecs.h>
#include <SDL3/SDL.h>
#include <vulkan/vulkan.h>
#include <functional>

module crude.engine;

import crude.core.logger;
import crude.core.memory;
import crude.scripts.window_script;
import crude.graphics.device;
import crude.platform.sdl_helper;
import crude.platform.sdl_window_container;
import crude.gui.imgui_helper;
import crude.gui.imgui_input_system;
import crude.scene.mesh;
import crude.scene.light;

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
  platform::deinitializeSDL();
  platform::unloadSDLVulkan();
}

void Engine::initialize(const Initialize& initialize)
{
  initializeWindow(initialize.window);
  initializeSystems();
}

void Engine::deinitialize()
{
}

void Engine::mainLoop()
{
  const auto windowScriptComponent = m_world.get<scripts::Window_Script_Component>();
  while (!windowScriptComponent->shouldClose)
  {
    m_world.progress();
  }
  m_rendererCoreCtx->device->waitIdle();
}

void Engine::initializeWindow(const Initialize_Window& initialize)
{
  m_windowContainer = crude::core::allocateShared<crude::platform::SDL_Window_Container>(
    initialize.title, initialize.width, initialize.height, crude::platform::SDL_WINDOW_CONTAINER_FLAG_VULKAN);
}

void Engine::initializeSystems()
{
  initializeInputSystems();
  initializeRendererSystems();

  m_sceneLoaderCtx = core::allocateShared<resources::Scene_Loader_Context>();
  m_sceneSaverCtx = core::allocateShared<resources::Scene_Saver_Context>();
  m_sceneSaverSystem = resources::registerSceneSaverSystem(m_world, m_sceneSaverCtx);
  m_sceneLoaderSystem = resources::registerSceneLoaderSystem(m_world, m_sceneLoaderCtx);

  m_gltfModelLoaderCtx = core::allocateShared<resources::GLTF_Model_Loader_Context>(m_rendererCoreCtx->transferCommandPool);
  m_gltfModelLoaderSystem = resources::registerGLTFModelLoaderSystem(m_world, m_gltfModelLoaderCtx);
}

void Engine::initializeInputSystems()
{
  m_world.set<scripts::Window_Script_Component>({});

  m_inputSystemCtx = core::allocateShared<platform::Input_System_Context>();
  m_inputSystemCtx->handleEventSystems = {
    m_world.system("WindowInputSystem")
      .kind(0)
      .ctx(m_inputSystemCtx.get())
      .run(scripts::windowScriptInputSystemProcess),
    m_world.system("ImguiUpdateEvent")
      .kind(0)
      .ctx(m_inputSystemCtx.get())
      .run(gui::imguiInputSystemProcess)
  };

  m_inputSystem = m_world.system("InputSystem")
    .ctx(m_inputSystemCtx.get())
    .kind(flecs::PreUpdate)
    .run(platform::inputSystemProcess);
}

void Engine::initializeRendererSystems()
{
  m_rendererCoreCtx = core::allocateShared<graphics::Renderer_Core_System_Ctx>(m_windowContainer);
  m_rendererFrameCtx = core::allocateShared<graphics::Renderer_Frame_System_Ctx>(m_rendererCoreCtx);
  m_rendererDeferredGBufferPassCtx = core::allocateShared<graphics::Renderer_Deferred_GBuffer_Pass_Systen_Ctx>(m_rendererFrameCtx);
  m_rendererLightCtx = core::allocateShared<graphics::Renderer_Light_Ctx>(m_rendererCoreCtx);
  m_rendererFullscreenPbrPassCtx = core::allocateShared<graphics::Renderer_Fullscreen_PBR_Pass_Ctx>(m_rendererFrameCtx, m_rendererLightCtx, m_rendererDeferredGBufferPassCtx->gbuffer);
  m_rendererImguiPassCtx = core::allocateShared<gui::Renderer_ImGui_Pass_System_Ctx>(m_rendererFrameCtx);

  flecs::system deferredGBufferPassSystem = m_world.system<core::shared_ptr<graphics::Mesh_Buffer>, core::shared_ptr<scene::Mesh>>("DeferredGBufferPassSystem")
    .ctx(m_rendererDeferredGBufferPassCtx.get())
    .kind(0)
    .run(graphics::rendererDeferredGBufferPassSystemProcess);

  m_lightUpdateSystem = m_world.system<scene::Point_Light_CPU>("LightUpdateSystem")
    .ctx(m_rendererLightCtx.get())
    .kind(flecs::OnUpdate)
    .run(graphics::rendererLightUpdateSystemProcess);

  flecs::system frameStartSystem = m_world.system("FrameStartSystem")
    .ctx(m_rendererFrameCtx.get())
    .kind(0)
    .run(graphics::rendererFrameStartSystemProcess);

  flecs::system fullscreenPBRPassSystem = m_world.system("FullscreenPBRPassSystem")
    .ctx(m_rendererFullscreenPbrPassCtx.get())
    .kind(0)
    .run(graphics::rendererFullscreenPBRPassSystemProcess);

  flecs::system imguiPassSystem = m_world.system("ImguiPassSystem")
    .ctx(m_rendererImguiPassCtx.get())
    .kind(0)
    .run(gui::rendererImguiPassSystemProcess);

  flecs::system frameSubmitSystem = m_world.system("FrameSubmitSystem")
    .ctx(m_rendererFrameCtx.get())
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
}

}