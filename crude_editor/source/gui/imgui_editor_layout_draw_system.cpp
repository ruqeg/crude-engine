#include <flecs.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <functional>
#include <DirectXMath.h>

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

Imgui_Editor_Layout_Draw_Ctx::Imgui_Editor_Layout_Draw_Ctx(crude::core::shared_ptr<crude::gui::ImGui_Texture_Descriptor_Set> sceneImguiTextureDescriptorSet)
  : sceneImguiTextureDescriptorSet{ sceneImguiTextureDescriptorSet }
  , nodeClickedIndex { -1 }
{}

void imguiEditorLayoutDrawSystemProcess(flecs::iter& it)
{
  Imgui_Editor_Layout_Draw_Ctx* editorLayoutCtx = it.ctx<Imgui_Editor_Layout_Draw_Ctx>();

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

  ImGui::Begin("Viewport");
  ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
  ImGui::Image(editorLayoutCtx->sceneImguiTextureDescriptorSet->getHandle(), ImVec2{ viewportPanelSize.x, viewportPanelSize.y });
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