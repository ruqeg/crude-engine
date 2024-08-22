#include <DirectXMath.h>
#include <flecs.h>
#include <SDL3/SDL.h>
#include <vulkan/vulkan.h>
#include <functional>

module crude.engine;

import crude.core.logger;
import crude.core.memory;
import crude.scene.free_camera_script;
import crude.scene.window_script;
import crude.scene.camera;
import crude.platform.sdl_helper;
import crude.platform.sdl_window_container;
import crude.platform.input_system;
import crude.resources.gltf_loader;
import crude.graphics.renderer_core_component;
import crude.graphics.deferred_gbuffer_pass_system;
import crude.graphics.fullscreen_pbr_pass_system;
import crude.graphics.renderer_frame_system;
import crude.graphics.device;
import crude.graphics.physical_device;
import crude.graphics.swap_chain;
import crude.graphics.swap_chain_image;
import crude.graphics.image_attachment;
import crude.graphics.queue;
import crude.graphics.surface;
import crude.graphics.instance;
import crude.graphics.render_pass;
import crude.graphics.subpass_dependency;
import crude.graphics.descriptor_pool;
import crude.graphics.format_helper;
import crude.graphics.framebuffer;
import crude.graphics.command_buffer;
import crude.network.network_system;
import crude.graphics.vulkan_utils;
import crude.gui.imgui_helper;

import crude.gui.imgui_renderer_pass_system;
import crude.gui.imgui_input_system;
import crude.gui.imgui_demo_layout_draw_system;

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

void Engine::initialize(const char* title, core::uint32 width, core::uint32 height)
{
  registerSystems();

  m_world.set<core::shared_ptr<platform::SDL_Window_Container>>(crude::core::allocateShared<crude::platform::SDL_Window_Container>(
    title, width, height, crude::platform::SDL_WINDOW_CONTAINER_FLAG_VULKAN));

  m_world.query().run(graphics::rendererCoreComponentInitialize);
  m_world.query().run(graphics::deferredGBufferPassSystemComponentInitialize);
  m_world.query().run(graphics::fullscreenPBRPassComponentInitialize);
  m_world.query().run(graphics::rendererFrameSystemComponentInitiailize);
  
  resources::GLTF_Loader gltfLoader(m_world.get_mut<graphics::Renderer_Core_Component>()->transferCommandPool, m_world);
  m_sceneNode = gltfLoader.loadNodeFromFile("../../crude_example/basic_triangle_examle/resources/sponza.glb");

  flecs::entity cameraNode = m_world.entity("camera node");
  cameraNode.set<scene::Camera>([width, height](){
    scene::Camera camera;
    camera.calculateViewToClipMatrix(DirectX::XM_PIDIV4, width / (core::float64)height, 0.05f, 100.0f);
    return camera;
  }());
  cameraNode.set<scene::Transform>([&cameraNode]() {
    scene::Transform transform(cameraNode);
    transform.setTranslation(0.0, 0.0, -2.0);
    return transform;
  }());
  cameraNode.set<scene::script::Free_Camera_Component>(scene::script::Free_Camera_Component());
  cameraNode.child_of(m_sceneNode);
  m_world.get_mut<graphics::Renderer_Frame_Component>()->cameraNode = cameraNode;
  
  m_world.query().run(gui::imguiRendererPassSystemInitialize);

  m_timer.setFrameRate(60);
}

void Engine::deinitialize()
{
  m_world.query().run(gui::imguiRendererPassSystemDeinitialize);
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
      update(elapsed);
      render();
    }
  }
  deinitialize();
}

void Engine::registerSystems()
{
  m_world.set<scene::script::Window_Component>({});

  m_world.set<platform::Input_System_Component>(platform::Input_System_Component({ 
    m_world.system<scene::script::Free_Camera_Component>("FreeCameraUpdateEvent")
      .kind(flecs::OnUpdate)
      .run(scene::script::freeCameraUpdateEventSystemProcess),
    m_world.system("WindowUpdateEvent")
      .kind(flecs::OnUpdate)
      .run(scene::script::windowUpdateEventSystemProcess),
    m_world.system("ImguiUpdateEvent")
      .kind(flecs::OnUpdate)
      .run(gui::imguiUpdateEventSystemProcess)
  }));

  m_world.set<gui::ImGui_Layout_Component>(gui::ImGui_Layout_Component({
    m_world.system("ImguiDemoLayoutDrawSystemProcess")
      .kind(flecs::PreStore)
      .run(gui::imguiDemoLayoutDrawSystemProcess),
  }));

  m_inputSystem = m_world.system("InputSystem")
    .kind(flecs::PreUpdate)
    .run(platform::inputSystemProcess);

  m_freeCameraUpdateSystem = m_world.system<scene::script::Free_Camera_Component, scene::Transform>("FreeCameraUpdateSystem")
    .kind(flecs::OnUpdate)
    .run(scene::script::freeCameraUpdateSystemProcess);

  m_deferredGBufferPassSystem = m_world.system<core::shared_ptr<graphics::Mesh_Buffer>, core::shared_ptr<scene::Mesh>>("DeferredGBufferPassSystem")
    .kind(flecs::PreStore)
    .run(graphics::deferredGBufferPassSystemProcess);

  m_fullscreenPBRPassSystem = m_world.system("FullscreenPBRPassSystem")
    .kind(flecs::PreStore)
    .run(graphics::fullscreenPBRPassSystemProcess);

  m_imguiRendererPassSystem = m_world.system("ImguiRendererPassSystem")
    .kind(flecs::PreStore)
    .run(gui::imguiRendererPassSystemProcess);

  m_rendererFrameStartSystem = m_world.system("RendererFrameStartSystem")
    .kind(flecs::PreStore)
    .run(graphics::rendererFrameStartSystemProcess);

  m_rendererFrameSubmitSystem = m_world.system("RendererFrameSubmitSystem")
    .kind(flecs::OnStore)
    .run(graphics::rendererFrameSubmitSystemProcess);
}

void Engine::update(core::float64 elapsed)
{
  m_freeCameraUpdateSystem.run(elapsed);
}

void Engine::render()
{
  m_rendererFrameStartSystem.run();
  m_deferredGBufferPassSystem.run();
  m_fullscreenPBRPassSystem.run();
  m_imguiRendererPassSystem.run();
  m_rendererFrameSubmitSystem.run();
}

}