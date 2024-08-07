#include <directxmath/DirectXMath.h>
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
import crude.graphics.deferred_pbr_pass_system;
import crude.graphics.renderer_frame_system;
import crude.graphics.device;
import crude.graphics.physical_device;
import crude.graphics.swap_chain;
import crude.graphics.swap_chain_image;
import crude.graphics.image_attachment;
import crude.graphics.format_helper;
import crude.network.network_system;

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
  
  graphics::Renderer_Core_Component rendererCoreComponent(windowContainer);
  m_world.set<graphics::Renderer_Core_Component>(rendererCoreComponent);
  m_world.set<graphics::Deferred_PBR_Pass_Component>({ 
    rendererCoreComponent.device, 
    rendererCoreComponent.swapchain->getExtent(), 
    static_cast<core::uint32>(rendererCoreComponent.swapchainImagesViews.size()) });
  m_world.set<graphics::Renderer_Frame_Component>(graphics::Renderer_Frame_Component(rendererCoreComponent.device, rendererCoreComponent.graphicsCommandPool));

  resources::GLTF_Loader gltfLoader(m_world.get_mut<graphics::Renderer_Core_Component>()->transferCommandPool, m_world);
  m_sceneNode = gltfLoader.loadNodeFromFile("../../crude_example/basic_triangle_examle/resources/sponza.glb");

  flecs::entity cameraNode = m_world.entity("camera node");
  cameraNode.set<scene::Camera>([windowContainer](){
    scene::Camera camera;
    camera.calculateViewToClipMatrix(DirectX::XM_PIDIV4, windowContainer->getAspect(), 0.05f, 100.0f);
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
      m_rendererFrameStartSystem.run();
      m_deferredPbrPassSystem.run();
      m_rendererFrameSubmitSystem.run();
    }
  }
}

void Engine::initializeSystems()
{
  m_world.set<scene::script::Window_Component>({});

  m_world.set<platform::Input_System_Component>(platform::Input_System_Component({ 
    m_world.system<scene::script::Free_Camera_Component>("FreeCameraUpdateEvent")
      .kind(flecs::OnUpdate)
      .run(scene::script::freeCameraUpdateEventSystemProcess),
    m_world.system("WindowUpdateEvent")
      .kind(flecs::OnUpdate)
      .run(scene::script::windowUpdateEventSystemProcess) 
  }));

  m_inputSystem = m_world.system("InputSystem")
    .kind(flecs::PreUpdate)
    .run(platform::inputSystemProcess);

  m_freeCameraUpdateSystem = m_world.system<scene::script::Free_Camera_Component, scene::Transform>("FreeCameraUpdateSystem")
    .kind(flecs::OnUpdate)
    .run(scene::script::freeCameraUpdateSystemProcess);

  m_deferredPbrPassSystem = m_world.system<core::shared_ptr<graphics::Mesh_Buffer>, core::shared_ptr<scene::Mesh>>("DeferredPbrPassSystem")
    .kind(flecs::OnUpdate)
    .run(graphics::deferredPbrPassSystemProcess);

  m_rendererFrameStartSystem = m_world.system("RendererFrameStartSystem")
    .kind(flecs::OnUpdate)
    .run(graphics::rendererFrameStartSystemProcess);

  m_rendererFrameSubmitSystem = m_world.system("RendererFrameSubmitSystem")
    .kind(flecs::OnUpdate)
    .run(graphics::rendererFrameSubmitSystemProcess);
}

}