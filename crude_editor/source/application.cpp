#include <flecs.h>
#include <DirectXMath.h>

module application;

import crude.gui.imgui_demo_layout_draw_system;
import crude.scripts.free_camera_script;
import crude.scene.camera;
import crude.resources.gltf_loader;
import crude.graphics.renderer_frame_system;
import crude.graphics.renderer_core_system;
import crude.graphics.command_pool;
import gui.imgui_editor_layout_draw_system;
import crude.graphics.renderer_core_system;
import crude.graphics.sampler_state;
import crude.graphics.image_view;
import crude.graphics.sampler;
import crude.gui.imgui_texture_descriptor_set;

void Application::initialize()
{
  Engine::initialize({
    .window = {
      .title = "TEST",
      .width = 1000,
      .height = 800
    },
    .systems = {
      .inputSystems = {
        m_world.system<crude::scripts::Free_Camera_Script_Component>("FreeCameraScriptInputSystem")
          .ctx(&m_inputSystemCtx)
          .kind(0)
          .run(crude::scripts::freeCameraScriptInputSystemProcess),
      },
      .imguiLayoutSystems = {
        m_world.system("ImguiEditorLayoutDrawSystem")
          .ctx(&m_editorLayoutCtx)
          .kind(0)
          .run(gui::imguiEditorLayoutDrawSystemProcess)
      }
    }
  });

  m_editorLayoutCtx.sceneImguiTextureDescriptorSet = crude::core::allocateShared<crude::gui::ImGui_Texture_Descriptor_Set>(
    crude::core::allocateShared<crude::graphics::Texture>(
      crude::core::allocateShared<crude::graphics::Image_View>(m_rendererSystemCtx.fullscreenPbrPassCtx.colorAttachment),
      crude::core::allocateShared<crude::graphics::Sampler>(m_rendererSystemCtx.coreCtx.device, crude::graphics::csamlper_state::gMagMinMipLinearRepeat)));

  m_freeCameraUpdateSystem = m_world.system<crude::scripts::Free_Camera_Script_Component, crude::scene::Transform>("FreeCameraScriptUpdateSystem")
    .kind(flecs::OnUpdate)
    .run(crude::scripts::freeCameraScriptUpdateSystemProcess);

  initializeScene(1000.0 / 800.0);
}

void Application::run()
{
  Engine::mainLoop();
}

void Application::deinitialize()
{
  m_editorLayoutCtx.sceneImguiTextureDescriptorSet.reset();
  Engine::deinitialize();
}

void Application::initializeScene(crude::core::float32 aspectRatio)
{
  crude::resources::GLTF_Loader gltfLoader(m_world, m_rendererSystemCtx.coreCtx.transferCommandPool);
  m_sceneNode = gltfLoader.loadNodeFromFile("../../../crude_editor/resources/sponza.glb");
  initializeCamera(aspectRatio);
}

void Application::initializeCamera(crude::core::float32 aspectRatio)
{
  flecs::entity cameraNode = m_world.entity("camera node");
  cameraNode.set<crude::scene::Camera>([aspectRatio]() {
    crude::scene::Camera camera;
    camera.calculateViewToClipMatrix(DirectX::XM_PIDIV4, aspectRatio, 0.05f, 100.0f);
    return camera;
    }());
  cameraNode.set<crude::scene::Transform>([&cameraNode]() {
    crude::scene::Transform transform(cameraNode);
    transform.setTranslation(0.0, 0.0, -2.0);
    return transform;
    }());
  cameraNode.set<crude::scripts::Free_Camera_Script_Component>(crude::scripts::Free_Camera_Script_Component());
  cameraNode.child_of(m_sceneNode);
  m_rendererSystemCtx.frameCtx.cameraNode = cameraNode;
}