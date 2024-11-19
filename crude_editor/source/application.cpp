#include <flecs.h>
#include <DirectXMath.h>
#include <nfd.h>

module crude.editor.application;

import crude.graphics.command_pool;
import crude.graphics.sampler_state;
import crude.graphics.image_view;
import crude.graphics.sampler;
import crude.graphics.texture;
import crude.gui.imgui_texture_descriptor_set;
import crude.resources.gltf_model_loader_system;
import crude.scene.camera;
import crude.scripts.free_camera_script;
import crude.gui.imgui_demo_layout_draw_system;

import crude.editor.gui.imgui_editor_layout_draw_system;

namespace crude::editor
{

void Application::initialize()
{
  NFD_Init();

  Engine::initialize({
    .window = {
      .title = "TEST",
      .width = 1000,
      .height = 800
    }
    });

  m_editorLayoutCtx = core::allocateShared<gui::Imgui_Editor_Layout_Draw_Ctx>();

  m_inputSystemCtx->handleEventSystems.push_back(
    m_world.system<scripts::Free_Camera_Script_Component>("FreeCameraScriptInputSystem")
    .ctx(m_inputSystemCtx.get())
    .kind(0)
    .run(scripts::freeCameraScriptInputSystemProcess));

  m_rendererImguiPassCtx->layoutsDrawSystems.push_back(
    m_world.system("ImguiEditorLayoutDrawSystem")
    .ctx(m_editorLayoutCtx.get())
    .kind(0)
    .run(gui::imguiEditorLayoutDrawSystemProcess));
  m_rendererImguiPassCtx->layoutsDrawSystems.push_back(
    m_world.system("imguiDemoLayoutDrawSystemProcess")
    .kind(0)
    .run(crude::gui::imguiDemoLayoutDrawSystemProcess));

  m_freeCameraUpdateSystem = m_world.system<scripts::Free_Camera_Script_Component, scene::Transform>("FreeCameraScriptUpdateSystem")
    .kind(flecs::OnUpdate)
    .run(scripts::freeCameraScriptUpdateSystemProcess);

  initializeScene(1000.0 / 800.0);
  initializeEditorCamera(1000.0 / 800.0);

  m_editorLayoutCtx->rendererFullscreenPbrPass = m_rendererFullscreenPbrPassCtx;
  m_editorLayoutCtx->sceneNode            = m_sceneNode;
  m_editorLayoutCtx->editorCameraNode     = m_rendererFrameCtx->cameraNode;
  m_editorLayoutCtx->editorSelectedNode   = m_sceneNode;
  m_editorLayoutCtx->viewportImguiTexture = core::allocateShared<crude::gui::ImGui_Texture_Descriptor_Set>(
    m_rendererImguiPassCtx,
    core::allocateShared<graphics::Texture>(
      core::allocateShared<graphics::Image_View>(m_rendererFullscreenPbrPassCtx->colorAttachment),
      core::allocateShared<graphics::Sampler>(m_rendererCoreCtx->device, graphics::csamlper_state::gMagMinMipLinearRepeat)));

  m_sceneLoaderCtx->callback = [this](flecs::entity newSceneNode) {
    m_sceneNode                           = newSceneNode;
    m_editorLayoutCtx->sceneNode          = m_sceneNode;
    m_editorLayoutCtx->editorSelectedNode = m_sceneNode;
    };
}

void Application::run()
{
  Engine::mainLoop();
}

void Application::deinitialize()
{
  NFD_Quit();
}

void Application::initializeScene(core::float32 aspectRatio)
{
  m_sceneNode = m_world.entity("scene 1");
  m_sceneNode.set<scene::Transform>(scene::Transform{ m_sceneNode });

  flecs::entity modelNode = m_world.entity("m_modelNode");
  modelNode.child_of(m_sceneNode);
  modelNode.set<crude::resources::GLTF_Model_Metadata_Component>({
    .path = "../../../crude_editor/resources/main1_sponza/NewSponza_Main_glTF_003.gltf"
  });
  modelNode.add<crude::resources::GLTF_Model_Loader_Uninitialized_Flag>();
}

void Application::initializeEditorCamera(core::float32 aspectRatio)
{
  flecs::entity cameraNode = m_world.entity("Editor Camera");
  cameraNode.set<scene::Camera>([aspectRatio]() {
    scene::Camera camera;
    camera.calculateViewToClipMatrix(DirectX::XM_PIDIV4, aspectRatio, 0.05f, 100.0f);
    return camera;
    }());
  cameraNode.set<scene::Transform>([&cameraNode]() {
    scene::Transform transform(cameraNode);
    transform.setTranslation(0.0, 0.0, -2.0);
    return transform;
    }());
  cameraNode.set<scripts::Free_Camera_Script_Component>(scripts::Free_Camera_Script_Component());
  m_rendererFrameCtx->cameraNode = cameraNode;
}

}