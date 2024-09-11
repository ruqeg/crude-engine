#include <flecs.h>
#include <DirectXMath.h>

module application;

import crude.graphics.command_pool;
import crude.graphics.sampler_state;
import crude.graphics.image_view;
import crude.graphics.sampler;
import crude.graphics.texture;
import crude.gui.imgui_texture_descriptor_set;
import crude.resources.gltf_loader;
import crude.scene.camera;
import crude.scripts.free_camera_script;
import crude.gui.imgui_demo_layout_draw_system;

import gui.imgui_editor_layout_draw_system;

void Application::initialize()
{
  Engine::initialize({
    .window = {
      .title = "TEST",
      .width = 1000,
      .height = 800
    }
  });

  m_editorLayoutCtx = crude::core::allocateShared<gui::Imgui_Editor_Layout_Draw_Ctx>(crude::core::allocateShared<crude::gui::ImGui_Texture_Descriptor_Set>(
    m_rendererImguiPassCtx,
    crude::core::allocateShared<crude::graphics::Texture>(
      crude::core::allocateShared<crude::graphics::Image_View>(m_rendererFullscreenPbrPassCtx->colorAttachment),
      crude::core::allocateShared<crude::graphics::Sampler>(m_rendererCoreCtx->device, crude::graphics::csamlper_state::gMagMinMipLinearRepeat))));

  m_inputSystemCtx->handleEventSystems.push_back(
    m_world.system<crude::scripts::Free_Camera_Script_Component>("FreeCameraScriptInputSystem")
      .ctx(m_inputSystemCtx.get())
      .kind(0)
      .run(crude::scripts::freeCameraScriptInputSystemProcess));

  m_rendererImguiPassCtx->layoutsDrawSystems.push_back(
    m_world.system("ImguiEditorLayoutDrawSystem")
      .ctx(m_editorLayoutCtx.get())
      .kind(0)
      .run(gui::imguiEditorLayoutDrawSystemProcess));
  m_rendererImguiPassCtx->layoutsDrawSystems.push_back(
    m_world.system("imguiDemoLayoutDrawSystemProcess")
    .kind(0)
    .run(crude::gui::imguiDemoLayoutDrawSystemProcess));
  
  m_freeCameraUpdateSystem = m_world.system<crude::scripts::Free_Camera_Script_Component, crude::scene::Transform>("FreeCameraScriptUpdateSystem")
    .kind(flecs::OnUpdate)
    .run(crude::scripts::freeCameraScriptUpdateSystemProcess);

  initializeScene(1000.0 / 800.0);

  m_lightUpdateSystem.run();

  m_editorLayoutCtx->sceneNode = m_sceneNode;
  m_editorLayoutCtx->cameraNode = m_rendererFrameCtx->cameraNode;
  m_editorLayoutCtx->selectedNode = m_editorLayoutCtx->cameraNode;
}

void Application::run()
{
  Engine::mainLoop();
}

void Application::deinitialize()
{
}

void Application::initializeScene(crude::core::float32 aspectRatio)
{
  crude::resources::GLTF_Loader gltfLoader(m_world, m_rendererCoreCtx->transferCommandPool);
  m_sceneNode = gltfLoader.loadNodeFromFile("../../../crude_editor/resources/sponza2.glb");
  initializeCamera(aspectRatio);
}

void Application::initializeCamera(crude::core::float32 aspectRatio)
{
  flecs::entity cameraNode = m_world.entity("Camera Node");
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
  m_rendererFrameCtx->cameraNode = cameraNode;
}