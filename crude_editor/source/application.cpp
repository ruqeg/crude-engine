#include <flecs.h>
#include <DirectXMath.h>

module application;

import crude.gui.imgui_demo_layout_draw_system;
import crude.scene.free_camera_script;
import crude.scene.camera;
import crude.resources.gltf_loader;
import crude.graphics.renderer_frame_system;

void Application::initialize()
{
  Engine::initialize({
    .window = {
      .title  = "TEST",
      .width  = 1000,
      .height = 800
    },
    .systems = {
      .inputSystems = {
        m_world.system<crude::scene::script::Free_Camera_Component>("FreeCameraUpdateEvent")
          .kind(0)
          .run(crude::scene::script::freeCameraUpdateEventSystemProcess),
      },
      .imguiLayoutSystems = {
        m_world.system("ImguiDemoLayoutDrawSystemProcess")
          .kind(0)
          .run(crude::gui::imguiDemoLayoutDrawSystemProcess),
      }
    }
  });

  m_freeCameraUpdateSystem = m_world.system<crude::scene::script::Free_Camera_Component, crude::scene::Transform>("FreeCameraUpdateSystem")
    .kind(flecs::OnUpdate)
    .run(crude::scene::script::freeCameraUpdateSystemProcess);

  initializeScene(1000.0 / 800.0);
}

void Application::run()
{
  Engine::mainLoop();
}

void Application::deinitialize()
{
  Engine::deinitialize();
}

void Application::initializeScene(crude::core::float32 aspectRatio)
{
  crude::resources::GLTF_Loader gltfLoader(m_world);
  m_sceneNode = gltfLoader.loadNodeFromFile("../../crude_editor/resources/sponza.glb");
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
  cameraNode.set<crude::scene::script::Free_Camera_Component>(crude::scene::script::Free_Camera_Component());
  cameraNode.child_of(m_sceneNode);
  m_world.get_mut<crude::graphics::Renderer_Frame_Component>()->cameraNode = cameraNode;
}