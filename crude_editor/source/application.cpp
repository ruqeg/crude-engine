#include <flecs.h>
#include <DirectXMath.h>
#include <nfd.h>

module crude.editor.application;

import crude.gfx.vk.command_pool;
import crude.gfx.vk.sampler_state;
import crude.gfx.vk.image_view;
import crude.gfx.vk.sampler;
import crude.gfx.texture;
import crude.resources.gltf_model_loader_system;
import crude.scene.camera;
import crude.scripts.free_camera_script;
import crude.gui.imgui_demo_layout_draw_system;
import crude.gfx.gbuffer_pass;

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

  m_freeCameraUpdateSystem = m_world.system<scripts::Free_Camera_Script_Component, scene::Transform>("FreeCameraScriptUpdateSystem")
    .kind(flecs::OnUpdate)
    .run(scripts::freeCameraScriptUpdateSystemProcess);

  initializeScene(1000.0 / 800.0);
  initializeEditorCamera(1000.0 / 800.0);

  m_editorLayoutCtx->sceneNode            = m_sceneNode;
  m_editorLayoutCtx->editorSelectedNode   = m_sceneNode;

  m_sceneLoaderCtx->callback = [this](flecs::entity newSceneNode) {
    m_sceneNode                           = newSceneNode;
    m_editorLayoutCtx->sceneNode          = m_sceneNode;
    m_editorLayoutCtx->editorSelectedNode = m_sceneNode;
    };

  gfx::initializeGbufferPass(m_graph, m_world, m_cameraNode);
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
  m_cameraNode = m_world.entity("Editor Camera");
  m_cameraNode.set<scene::Camera>([aspectRatio]() {
    scene::Camera camera;
    camera.calculateViewToClipMatrix(DirectX::XM_PIDIV4, aspectRatio, 0.05f, 100.0f);
    return camera;
    }());
  m_cameraNode.set<scene::Transform>([&cameraNode = m_cameraNode]() {
    scene::Transform transform(cameraNode);
    transform.setTranslation(0.0, 0.0, -2.0);
    return transform;
    }());
  m_cameraNode.set<scripts::Free_Camera_Script_Component>(scripts::Free_Camera_Script_Component());
}

}