#include <flecs.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <functional>
#include <DirectXMath.h>
#include <ImGuizmo/ImGuizmo.h>

module gui.imgui_editor_layout_draw_system;

import crude.core.logger;
import crude.core.alias;
import crude.graphics.texture;
import crude.scene.transform;
import crude.scene.camera;
import crude.scene.light;
import crude.gui.imgui_texture_descriptor_set;
import crude.scripts.free_camera_script;

namespace gui
{

ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);

void TransformStart(Imgui_Editor_Layout_Draw_Ctx* editorLayoutCtx)
{

}

void EditTransform(Imgui_Editor_Layout_Draw_Ctx* editorLayoutCtx)
{
}


Imgui_Editor_Layout_Draw_Ctx::Imgui_Editor_Layout_Draw_Ctx(crude::core::shared_ptr<crude::gui::ImGui_Texture_Descriptor_Set> sceneImguiTextureDescriptorSet)
  : sceneImguiTextureDescriptorSet{ sceneImguiTextureDescriptorSet }
  , nodeClickedIndex { -1 }
{}

void imguiEditorLayoutDrawSystemProcess(flecs::iter& it)
{
  Imgui_Editor_Layout_Draw_Ctx* editorLayoutCtx = it.ctx<Imgui_Editor_Layout_Draw_Ctx>();

  bool isPerspective = true;
  ImGuizmo::SetOrthographic(!isPerspective);
  ImGuizmo::BeginFrame();

  if (ImGuizmo::IsUsing())
  {
    ImGui::Text("Using gizmo");
  }
  else
  {
    ImGui::Text(ImGuizmo::IsOver() ? "Over gizmo" : "");
    ImGui::SameLine();
    ImGui::Text(ImGuizmo::IsOver(ImGuizmo::TRANSLATE) ? "Over translate gizmo" : "");
    ImGui::SameLine();
    ImGui::Text(ImGuizmo::IsOver(ImGuizmo::ROTATE) ? "Over rotate gizmo" : "");
    ImGui::SameLine();
    ImGui::Text(ImGuizmo::IsOver(ImGuizmo::SCALE) ? "Over scale gizmo" : "");
  }

  ImGui::DockSpaceOverViewport(0u, ImGui::GetMainViewport());

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

    nodeIndex++;
    if (nodeOpen)
    {
      node.children(drawNode);
      ImGui::TreePop();
    }
    };
  drawNode(editorLayoutCtx->sceneNode);
  ImGui::End();

  static ImGuiWindowFlags viewportWindowFlags = 0;
  ImGui::Begin("Viewport", nullptr, viewportWindowFlags);
  ImGuizmo::SetDrawlist();
  float windowWidth = (float)ImGui::GetWindowWidth();
  float windowHeight = (float)ImGui::GetWindowHeight();
  ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
  ImGuiWindow* window = ImGui::GetCurrentWindow();
  viewportWindowFlags = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(window->InnerRect.Min, window->InnerRect.Max) ? ImGuiWindowFlags_NoMove : 0;
  ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
  ImGui::Image(editorLayoutCtx->sceneImguiTextureDescriptorSet->getHandle(), ImVec2{ viewportPanelSize.x, viewportPanelSize.y });
  ImGui::SetCursorPos(ImGui::GetWindowContentRegionMin());

  auto camera = editorLayoutCtx->cameraNode.get_mut<crude::scene::Camera>();
  auto cameraTransform = editorLayoutCtx->cameraNode.get_mut<crude::scene::Transform>();
  auto nodeTransform = editorLayoutCtx->selectedNode.get_mut<crude::scene::Transform>();
  DirectX::XMFLOAT4X4 viewToClip = camera->getViewToClipFloat4x4();
  DirectX::XMFLOAT4X4 worldToView = cameraTransform->getWorldToNodeFloat4x4();
  DirectX::XMFLOAT4X4 nodeToWorld = nodeTransform->getNodeToWorldFloat4x4();

  ImGuizmo::SetID(0);
  ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
  ImGuizmo::Manipulate(&worldToView.m[0][0], &viewToClip.m[0][0], mCurrentGizmoOperation, mCurrentGizmoMode, &nodeToWorld.m[0][0], NULL, NULL);

  DirectX::XMFLOAT3 trans, rot, scale;
  ImGuizmo::DecomposeMatrixToComponents(&nodeToWorld.m[0][0], &trans.x, &rot.x, &scale.x);
  nodeTransform->setRotation(rot);
  nodeTransform->setScale(scale);
  nodeTransform->setTranslation(trans);

  ImGui::End();

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
}

}