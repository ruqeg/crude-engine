#include <DirectXMath.h>
#include <flecs.h>
#include <SDL3/SDL.h>
#include <vulkan/vulkan.h>
#include <functional>


#include <crude_shaders/deferred_gbuffer.frag.inl>
#include <crude_shaders/deferred_gbuffer.mesh.inl>
#include <crude_shaders/deferred_gbuffer.task.inl>


module crude.engine;

import crude.core.logger;
import crude.core.memory;
import crude.scripts.window_script;
import crude.gfx.vk.device;
import crude.platform.sdl_helper;
import crude.platform.sdl_window_container;
import crude.gui.imgui_helper;
import crude.gui.imgui_input_system;
import crude.scene.mesh;
import crude.scene.light;



import crude.gfx.vk.format_helper;
import crude.gfx.render_graph;
import crude.gfx.vk.shader_module;
import crude.gfx.vk.shader_stage_create_info;
import crude.gfx.vk.push_constant_range;
import crude.gfx.vk.color_blend_state_create_info;
import crude.gfx.vk.image_descriptor;
import crude.gfx.mesh_buffer;
import crude.gfx.material;
import crude.gfx.camera_ubo;
import crude.gfx.texture;

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
    m_graph->render();
  }
  m_rendererCore->getDevice()->waitIdle();
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

  m_gltfModelLoaderCtx = core::allocateShared<resources::GLTF_Loader_Context>(resources::GLTF_Loader_Context{
    .transferCommandPool = m_rendererCore->getTransferCommandPool(),
    .callback = [](flecs::entity entity) {
    }});
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
  m_rendererCore = core::allocateShared<gfx::Renderer_Core>(m_windowContainer);
  m_rendererFrame = core::allocateShared<gfx::Renderer_Frame>(m_rendererCore, 2u);
  m_graph = core::allocateShared<gfx::Render_Graph>(m_rendererFrame, 3u);

  //auto renderer = Util::make_handle<RenderPassSceneRenderer>();

  //RenderPassSceneRenderer::Setup setup = {};
  //setup.scene           = &scene_loader.get_scene();
  //setup.deferred_lights = &deferred_lights;
  //setup.context         = &context;
  //setup.suite           = &renderer_suite;
  //setup.flags           = SCENE_RENDERER_DEFERRED_GBUFFER_BIT;
  ////if (!config.clustered_lights && config.deferred_clustered_stencil_culling)
  ////  setup.flags |= SCENE_RENDERER_DEFERRED_GBUFFER_LIGHT_PREPASS_BIT;
  ////if (config.debug_probes)
  ////  setup.flags |= SCENE_RENDERER_DEBUG_PROBES_BIT;

  //renderer->init(setup);

  //gbuffer.set_render_pass_interface(std::move(renderer));

  //gbuffer->build();

  ////auto lighting = graph.addPass("lighting", VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT);
  ////lighting->addColorOutput("HDR", emissive, "emissive");
  ////lighting->addAttachmentInput("albedo");
  ////lighting->addAttachmentInput("normal");
  ////lighting->addAttachmentInput("pbr");
  ////lighting->addAttachmentInput("depth");
  ////lighting->setDepthStencilInput("depth");

  ////lighting->addTextureInput("shadow-main");
  ////lighting->addTextureInput("shadow-near");
}

}