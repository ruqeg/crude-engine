#include <flecs.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <functional>
#include <DirectXMath.h>
#include <ImGuizmo/ImGuizmo.h>
#include <algorithm>
#include <format>

module gui.imgui_editor_layout_draw_system;

import crude.core.logger;
import crude.core.alias;
import crude.graphics.texture;
import crude.scene.transform;
import crude.scene.camera;
import crude.scene.light;
import crude.gui.imgui_texture_descriptor_set;
import crude.scripts.free_camera_script;
import crude.core.std_containers_heap;

// TODO refactore this code in some day, but for now...

namespace gui
{

enum class ComponentType
{
  Transform,
  FreeCameraScript,
  Camera,
  PointLight
};

constexpr crude::core::array<ComponentType, 4> cComponentsTypes =
{
  ComponentType::Transform,
  ComponentType::FreeCameraScript,
  ComponentType::Camera,
  ComponentType::PointLight
};

const char* componentTypeToStr(ComponentType componentType)
{
  switch (componentType)
  {
  case ComponentType::Transform: return "Transform";
  case ComponentType::FreeCameraScript: return "Free Camera Script";
  case ComponentType::PointLight: return "Point Light";
  case ComponentType::Camera: return "Camera";
  default: return "Unkown";
  };
}

void addComponentToNode(flecs::entity node, ComponentType componentType)
{
  switch (componentType)
  {
  case ComponentType::Transform:
    if (!node.has<crude::scene::Transform>())
    {
      node.set<crude::scene::Transform>(crude::scene::Transform{ node });
    }
    break;
  case ComponentType::FreeCameraScript:
    if (!node.has<crude::scripts::Free_Camera_Script_Component>())
    {
      node.set<crude::scripts::Free_Camera_Script_Component>(crude::scripts::Free_Camera_Script_Component());
    }
    break;
  case ComponentType::PointLight:
    if (!node.has<crude::scene::Point_Light_CPU>())
    {
      node.set<crude::scene::Point_Light_CPU>(crude::scene::Point_Light_CPU());
    }
    break;
  case ComponentType::Camera:
    if (!node.has<crude::scene::Camera>())
    {
      node.set<crude::scene::Camera>(crude::scene::Camera());
    }
    break;
  default:
    break;
  };
}

Imgui_Editor_Layout_Draw_Ctx::Imgui_Editor_Layout_Draw_Ctx(crude::core::shared_ptr<crude::gui::ImGui_Texture_Descriptor_Set> sceneImguiTextureDescriptorSet)
  : sceneImguiTextureDescriptorSet{ sceneImguiTextureDescriptorSet }
  , nodeClickedIndex { -1 }
{}

void imguiEditorLayoutDrawSystemProcess(flecs::iter& it)
{
  Imgui_Editor_Layout_Draw_Ctx* editorLayoutCtx = it.ctx<Imgui_Editor_Layout_Draw_Ctx>();

  auto preDraw = []() {
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    constexpr bool isPerspective = true;
    ImGuizmo::SetOrthographic(!isPerspective);
    ImGuizmo::BeginFrame();

    ImGui::DockSpaceOverViewport(0u, ImGui::GetMainViewport());
    };

  auto drawSceneWindow = [editorLayoutCtx]() {
    ImGui::Begin("Scene");
    crude::core::size_t nodeIndex = 0;
    auto drawNode = [editorLayoutCtx, &nodeIndex](this const auto& drawNode, flecs::entity node) {
      ImGuiTreeNodeFlags treeNodeFlags =
        ImGuiTreeNodeFlags_OpenOnArrow |
        ImGuiTreeNodeFlags_OpenOnDoubleClick |
        ImGuiTreeNodeFlags_SpanAvailWidth;

      if (editorLayoutCtx->nodeClickedIndex == nodeIndex)
      {
        treeNodeFlags |= ImGuiTreeNodeFlags_Selected;
      }

      bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)nodeIndex, treeNodeFlags, node.name());
      if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
      {
        editorLayoutCtx->selectedNode = node;
        editorLayoutCtx->nodeClickedIndex = nodeIndex;
      }

      if (ImGui::BeginDragDropSource())
      {
        ImGui::SetDragDropPayload(node.name(), NULL, 0);
        ImGui::Text(node.name());
        ImGui::EndDragDropSource();
      }

     /* if (ImGui::Button("Select.."))
        ImGui::OpenPopup("my_select_popup");
      ImGui::SameLine();
      ImGui::TextUnformatted(selected_fish == -1 ? "<None>" : names[selected_fish]);
      if (ImGui::BeginPopup("my_select_popup"))
      {
        ImGui::SeparatorText("Aquarium");
        for (int i = 0; i < IM_ARRAYSIZE(names); i++)
          if (ImGui::Selectable(names[i]))
            selected_fish = i;
        ImGui::EndPopup();
      }*/

      std::string popupID = std::format("NodePopUp {}", node.name().c_str());

      if (ImGui::IsItemClicked(1) && !ImGui::IsItemToggledOpen())
      {
        ImGui::OpenPopup(popupID.c_str());
      }

      if (ImGui::BeginPopup(popupID.c_str()))
      {
        if (ImGui::BeginMenu("Add Component"))
        {
          for (ComponentType componentType : cComponentsTypes)
          {
            if (ImGui::MenuItem(componentTypeToStr(componentType)))
            {
              addComponentToNode(node, componentType);
            }
          }
          ImGui::EndMenu();
        }

        if (ImGui::MenuItem("Add Node"))
        {
          static crude::core::uint16 uniqueIndex = 0;
          flecs::entity newEntity = node.world().entity(std::format("Added Node {}", uniqueIndex++).c_str());
          newEntity.set<crude::scene::Transform>(crude::scene::Transform(newEntity));
          newEntity.child_of(node);
        }
        ImGui::EndPopup();
      }

      nodeIndex++;
      if (nodeOpen)
      {
        node.children(drawNode);
        ImGui::TreePop();
      }
      };
    drawNode(editorLayoutCtx->sceneNode);

    ImGui::End();
    };

  auto drawViewportWindow = [editorLayoutCtx]() {
    static ImGuizmo::OPERATION currentGizmoOperation{ ImGuizmo::TRANSLATE };
    static ImGuizmo::MODE currentGizmoMode{ ImGuizmo::WORLD };

    ImGui::Begin("Viewport");
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

    // draw scene texture
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    ImGui::Image(editorLayoutCtx->sceneImguiTextureDescriptorSet->getHandle(), ImVec2{ viewportPanelSize.x, viewportPanelSize.y });

    // settings manipulate
    ImGui::SetCursorPos(ImGui::GetWindowContentRegionMin());
    if (ImGui::IsKeyPressed(ImGuiKey_Z))
      currentGizmoOperation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(ImGuiKey_X))
      currentGizmoOperation = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed(ImGuiKey_C)) // r Key
      currentGizmoOperation = ImGuizmo::SCALE;
    if (ImGui::RadioButton("Translate", currentGizmoOperation == ImGuizmo::TRANSLATE))
      currentGizmoOperation = ImGuizmo::TRANSLATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", currentGizmoOperation == ImGuizmo::ROTATE))
      currentGizmoOperation = ImGuizmo::ROTATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", currentGizmoOperation == ImGuizmo::SCALE))
      currentGizmoOperation = ImGuizmo::SCALE;

    if (currentGizmoOperation != ImGuizmo::SCALE)
    {
      if (ImGui::RadioButton("Local", currentGizmoMode == ImGuizmo::LOCAL))
        currentGizmoMode = ImGuizmo::LOCAL;
      ImGui::SameLine();
      if (ImGui::RadioButton("World", currentGizmoMode == ImGuizmo::WORLD))
        currentGizmoMode = ImGuizmo::WORLD;
    }

    // imguizmo manipulate
    ImGui::SetCursorPos(ImGui::GetWindowContentRegionMin());
    auto camera = editorLayoutCtx->cameraNode.get_mut<crude::scene::Camera>();
    auto cameraTransform = editorLayoutCtx->cameraNode.get_mut<crude::scene::Transform>();
    auto selectedNodeTransform = editorLayoutCtx->selectedNode.get_mut<crude::scene::Transform>();
    const DirectX::XMFLOAT4X4 cameraViewToClip = camera->getViewToClipFloat4x4();
    const DirectX::XMFLOAT4X4 cameraWorldToView = cameraTransform->getWorldToNodeFloat4x4();
    DirectX::XMFLOAT4X4 selectedNodeToWorld = selectedNodeTransform->getNodeToWorldFloat4x4();

    ImGuizmo::SetID(0);
    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
    ImGuizmo::Manipulate(&cameraWorldToView._11, &cameraViewToClip._11, currentGizmoOperation, currentGizmoMode, &selectedNodeToWorld._11, NULL, NULL);

    // !TODO just use !=
    bool selectedNodeToWorldNotEqual = false;
    for (size_t i = 0; i < 4; ++i)
    {
      for (size_t k = 0; k < 4; ++k)
      {
        if (std::abs(selectedNodeTransform->getNodeToWorldFloat4x4().m[i][k] - selectedNodeToWorld.m[i][k]) > 0.0001f)
        {
          selectedNodeToWorldNotEqual = true;
          break;
        }
      }
    }
    if (selectedNodeToWorldNotEqual)
    {
      selectedNodeTransform->setNodeToParent(selectedNodeToWorld);
    }
    ImGui::End();
    };

  auto drawInspectorWindow = [editorLayoutCtx]() {
    ImGui::Begin("Inspector");
    if (editorLayoutCtx->selectedNode.is_valid())
    {
      ImGui::Text(editorLayoutCtx->selectedNode.name());

      auto transform = editorLayoutCtx->selectedNode.get_mut<crude::scene::Transform>();
      if (transform && ImGui::CollapsingHeader("Transform"))
      {
        auto translation = transform->getTranslationFloat3();
        auto scale = transform->getScaleFloat3();
        ImGui::DragFloat3("Translation", &translation.x, .1);
        ImGui::DragFloat3("Scale", &scale.x, .1);
        transform->setTranslation(translation);
        transform->setScale(scale);
      }

      // !TODO :D
      auto freeCameraScript = editorLayoutCtx->selectedNode.get_mut<crude::scripts::Free_Camera_Script_Component>();
      if (freeCameraScript && ImGui::CollapsingHeader("Free Camera Script"))
      {
        ImGui::DragFloat3("Moving Speed", &freeCameraScript->movingSpeedMultiplier.x, .1);
        ImGui::DragFloat2("Rotating Speed", &freeCameraScript->rotatingSpeedMultiplier.x, .1);
      }

      // !TODO :D
      auto camera = editorLayoutCtx->selectedNode.get_mut<crude::scene::Camera>();
      if (camera && ImGui::CollapsingHeader("Camera"))
      {
        crude::core::float32 farZ = camera->getFarZ();
        crude::core::float32 nearZ = camera->getNearZ();
        crude::core::float32 fov = camera->getFovRadians();
        ImGui::InputFloat("Far Z", &farZ);
        ImGui::InputFloat("Near Z", &nearZ);
        ImGui::SliderAngle("FOV", &fov);
        if (farZ != camera->getFarZ() || nearZ != camera->getNearZ() || fov != camera->getFovRadians())
        {
          camera->calculateViewToClipMatrix(fov, camera->getAspectRatio(), nearZ, farZ);
        }
      }

      // !TODO :D
      auto pointLightCPU = editorLayoutCtx->selectedNode.get_mut<crude::scene::Point_Light_CPU>();
      if (pointLightCPU && ImGui::CollapsingHeader("Point Light"))
      {
        // !TODO :D
        ImGui::ColorEdit4("Color", &pointLightCPU->intensity.x);
        ImGui::DragFloat3("Intensity", &pointLightCPU->intensity.x, .1);
        ImGui::InputFloat("Source Radius", &pointLightCPU->sourceRadius, .1);
      }
    }
    ImGui::End();
    };

  preDraw();
  drawSceneWindow();
  drawViewportWindow();
  drawInspectorWindow();
}

}