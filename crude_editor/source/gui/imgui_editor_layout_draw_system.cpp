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
#include <nfd.h>
#include <fstream>
#include <nlohmann/json.hpp>

module crude.editor.gui.imgui_editor_layout_draw_system;

import crude.core.std_containers_stack;
import crude.core.logger;
import crude.core.alias;
import crude.gfx.texture;
import crude.scene.transform;
import crude.scene.camera;
import crude.scene.light;
import crude.scripts.free_camera_script;
import crude.core.std_containers_heap;
import crude.resources.scene_loader_saver_system;

// TODO refactor this code in some day, but for now...

namespace crude::editor::gui
{

enum class Component_Type
{
  Transform,
  FreeCameraScript,
  Camera,
  PointLight
};

constexpr crude::core::array<Component_Type, 4> cComponentsTypes =
{
  Component_Type::Transform,
  Component_Type::FreeCameraScript,
  Component_Type::Camera,
  Component_Type::PointLight
};

const char* componentTypeToStr(Component_Type componentType)
{
  switch (componentType)
  {
  case Component_Type::Transform: return "Transform";
  case Component_Type::FreeCameraScript: return "Free Camera Script";
  case Component_Type::PointLight: return "Point Light";
  case Component_Type::Camera: return "Camera";
  default: return "Unkown";
  };
}

void addComponentToNode(flecs::entity node, Component_Type componentType)
{
  switch (componentType)
  {
  case Component_Type::Transform:
    if (!node.has<crude::scene::Transform>())
    {
      node.set<crude::scene::Transform>(crude::scene::Transform{ node });
    }
    break;
  case Component_Type::FreeCameraScript:
    if (!node.has<crude::scripts::Free_Camera_Script_Component>())
    {
      node.set<crude::scripts::Free_Camera_Script_Component>(crude::scripts::Free_Camera_Script_Component());
    }
    break;
  case Component_Type::PointLight:
    if (!node.has<crude::scene::Point_Light_CPU>())
    {
      node.set<crude::scene::Point_Light_CPU>(crude::scene::Point_Light_CPU());
    }
    break;
  case Component_Type::Camera:
    if (!node.has<crude::scene::Camera>())
    {
      node.set<crude::scene::Camera>(crude::scene::Camera());
    }
    break;
  default:
    break;
  };
}

void imguiEditorLayoutDrawSystemProcess(flecs::iter& it)
{
  Imgui_Editor_Layout_Draw_Ctx* editorLayoutCtx = it.ctx<Imgui_Editor_Layout_Draw_Ctx>();
  preDraw(editorLayoutCtx);
  drawMainMenuBar(editorLayoutCtx);
  drawInspectorWindow(editorLayoutCtx->editorSelectedNode);
  drawViewportWindow(editorLayoutCtx);
  drawSceneWindow(editorLayoutCtx);

 /* ImGui::Begin("Debug");
  if (ImGui::CollapsingHeader("Shading Model"))
  {
    gfx::PBRDebug& pbrDebug = editorLayoutCtx->rendererFullscreenPbrPass->pbrDebug;

    if (ImGui::TreeNode("PBR"))
    {
      ImGui::InputFloat3("Diffuse Coefficient", &pbrDebug.diffCoeff.x);
      if (ImGui::InputFloat("Diffuse Coefficient", &pbrDebug.diffCoeff.z)) pbrDebug.diffCoeff.x = pbrDebug.diffCoeff.y = pbrDebug.diffCoeff.z;
      ImGui::InputFloat3("Specular Coefficient", &pbrDebug.specCoeff.x);
      if (ImGui::InputFloat("Specular Coefficient", &pbrDebug.specCoeff.z)) pbrDebug.specCoeff.x = pbrDebug.specCoeff.y = pbrDebug.specCoeff.z;
      ImGui::Spacing();
      constexpr core::array<const char*, 4> ndfTypes = { "Blinn-Phong", "Beckmann", "GGX (Trowbridge-Reitz)", "Constant" };
      ImGui::Combo("NDF Type", &pbrDebug.ndfIndex, ndfTypes.data(), ndfTypes.size());
      if (pbrDebug.ndfIndex == (ndfTypes.size() - 1))
      {
        ImGui::InputFloat("NDF Constant", &pbrDebug.ndfConstant);
      }
      constexpr core::array<const char*, 8> gcTypes = { "Implicit", "Neumann", "Cook-Torrance", "Kelemen", "Beckmann", "GGX", "Schlick-Beckmann", "Constant" };
      ImGui::Combo("GS Type", &pbrDebug.gsIndex, gcTypes.data(), gcTypes.size());
      if (pbrDebug.gsIndex == (gcTypes.size() - 1))
      {
        ImGui::InputFloat("GS Constant", &pbrDebug.gsConstant);
      }
      constexpr core::array<const char*, 4> fsTypes = { "Default (None)", "Schlick", "Cook-Torrance", "Constant" };
      ImGui::Combo("FS Type", &pbrDebug.fsIndex, fsTypes.data(), fsTypes.size());
      if (pbrDebug.fsIndex == (fsTypes.size() - 1))
      {
        ImGui::InputFloat3("FS Constant", &pbrDebug.fsConstant.x);
      }
      ImGui::TreePop();
    }
  }
  ImGui::End();*/
}

void preDraw(Imgui_Editor_Layout_Draw_Ctx* layoutCtx)
{
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigWindowsMoveFromTitleBarOnly = true;

  constexpr bool isPerspective = true;
  ImGuizmo::SetOrthographic(!isPerspective);
  ImGuizmo::BeginFrame();

  ImGui::DockSpaceOverViewport(0u, ImGui::GetMainViewport());
}

void drawMainMenuBar(Imgui_Editor_Layout_Draw_Ctx* layoutCtx)
{
  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("Scene"))
    {
      if (ImGui::MenuItem("New")) {}
      if (ImGui::MenuItem("Open", "Ctrl+O"))
      {
        nfdu8char_t* outPath;
        crude::core::array<nfdu8filteritem_t, 1> filters = { { "Crude Scene", "crude" } };
        nfdopendialogu8args_t args = {
          .filterList = filters.data(),
          .filterCount = filters.size(),
        };

        nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);
        if (result == NFD_OKAY)
        {
          layoutCtx->sceneNode.set<crude::resources::Scene_Loader_Component>({.path = outPath });
          NFD_FreePathU8(outPath);
        }
        else if (result == NFD_CANCEL)
        {
          //
        }
        else
        {
          //NFD_GetError();
        }
      }
      if (ImGui::MenuItem("Save", "Ctrl+S"))
      {
        nfdu8char_t* outPath;
        crude::core::array<nfdu8filteritem_t, 1> filters = { { "Crude Scene", "crude" } };
        nfdsavedialogu8args_t args = {
          .filterList = filters.data(),
          .filterCount = filters.size(),
        };

        nfdresult_t result = NFD_SaveDialogU8_With(&outPath, &args);
        if (result == NFD_OKAY)
        {
          layoutCtx->sceneNode.set<crude::resources::Scene_Saver_Component>({ .path = outPath });
          NFD_FreePathU8(outPath);
        }
        else if (result == NFD_CANCEL)
        {
          //
        }
        else
        {
          //NFD_GetError();
        }
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}

void drawInspectorWindow(flecs::entity editorSelectedNode)
{
  // !TODO
  ImGui::Begin("Inspector");
  if (editorSelectedNode.is_valid())
  {
    ImGui::Text(editorSelectedNode.name());

    auto transform = editorSelectedNode.get_mut<crude::scene::Transform>();
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
    auto freeCameraScript = editorSelectedNode.get_mut<crude::scripts::Free_Camera_Script_Component>();
    if (freeCameraScript && ImGui::CollapsingHeader("Free Camera Script"))
    {
      ImGui::DragFloat3("Moving Speed", &freeCameraScript->movingSpeedMultiplier.x, .1);
      ImGui::DragFloat2("Rotating Speed", &freeCameraScript->rotatingSpeedMultiplier.x, .1);
    }

    // !TODO :D
    auto camera = editorSelectedNode.get_mut<crude::scene::Camera>();
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
    auto pointLightCPU = editorSelectedNode.get_mut<crude::scene::Point_Light_CPU>();
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

void drawViewportWindow(Imgui_Editor_Layout_Draw_Ctx* layoutCtx)
{
  static ImGuizmo::OPERATION currentGizmoOperation{ ImGuizmo::TRANSLATE };
  static ImGuizmo::MODE currentGizmoMode{ ImGuizmo::WORLD };

  ImGui::Begin("Viewport");
  ImGuizmo::SetDrawlist();
  ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

  // draw scene texture
  ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
  //ImGui::Image(layoutCtx->viewportImguiTexture->getHandle(), ImVec2{ viewportPanelSize.x, viewportPanelSize.y });

  auto camera = layoutCtx->editorCameraNode.get_mut<crude::scene::Camera>();
  auto cameraTransform = layoutCtx->editorCameraNode.get_mut<crude::scene::Transform>();
  auto selectedNodeTransform = layoutCtx->editorSelectedNode.get_mut<crude::scene::Transform>();

  if (layoutCtx->editorSelectedNode.is_valid() && camera && cameraTransform && selectedNodeTransform)
  {
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
    const DirectX::XMFLOAT4X4 cameraViewToClip = camera->getViewToClipFloat4x4();
    DirectX::XMFLOAT4X4 selectedNodeToParent = selectedNodeTransform->getNodeToParentFloat4x4();
    DirectX::XMFLOAT4X4 selectedParentToCameraView;
    DirectX::XMStoreFloat4x4(&selectedParentToCameraView, DirectX::XMMatrixMultiply(selectedNodeTransform->getParentToWorldMatrix(), cameraTransform->getWorldToNodeMatrix()));

    ImGuizmo::SetID(0);
    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
    ImGuizmo::Manipulate(&selectedParentToCameraView._11, &cameraViewToClip._11, currentGizmoOperation, currentGizmoMode, &selectedNodeToParent._11, NULL, NULL);

    // !TODO just use !=
    bool selectedNodeToWorldNotEqual = false;
    for (size_t i = 0; i < 4; ++i)
    {
      for (size_t k = 0; k < 4; ++k)
      {
        if (std::abs(selectedNodeTransform->getNodeToParentFloat4x4().m[i][k] - selectedNodeToParent.m[i][k]) > 0.0001f)
        {
          selectedNodeToWorldNotEqual = true;
          break;
        }
      }
    }
    if (selectedNodeToWorldNotEqual)
    {
      selectedNodeTransform->setNodeToParent(selectedNodeToParent);
    }
  }
  ImGui::End();
}

void drawSceneWindow(Imgui_Editor_Layout_Draw_Ctx* layoutCtx)
{
  // !TODO
  core::size_t currentNodeIndex = 0u;

  auto drawSceneNode = [layoutCtx, &currentNodeIndex](this const auto& drawSceneNode, flecs::entity node) {
    static core::size_t selectedNodeIndex = 0u;

    ImGuiTreeNodeFlags imguiTreeNodeFlags =
      ImGuiTreeNodeFlags_OpenOnArrow |
      ImGuiTreeNodeFlags_OpenOnDoubleClick |
      ImGuiTreeNodeFlags_SpanAvailWidth;

    // Mark selected node in imgui
    if (selectedNodeIndex == currentNodeIndex)
      imguiTreeNodeFlags |= ImGuiTreeNodeFlags_Selected;

    // Check if this node has been selected
    bool isTreeNodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)currentNodeIndex, imguiTreeNodeFlags, node.name());
    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
    {
      layoutCtx->editorSelectedNode = node;
      selectedNodeIndex = currentNodeIndex;
    }

    // Proccess node popup
    const std::string popupID = std::format("NodePopUp {}", node.name().c_str());

    constexpr core::uint32 rightClickImguiIndex = 1u;
    if (ImGui::IsItemClicked(rightClickImguiIndex) && !ImGui::IsItemToggledOpen())
    {
      ImGui::OpenPopup(popupID.c_str());
    }

    if (ImGui::BeginPopup(popupID.c_str()))
    {
      // Add component
      if (ImGui::BeginMenu("Add Component"))
      {
        for (Component_Type componentType : cComponentsTypes)
        {
          if (ImGui::MenuItem(componentTypeToStr(componentType)))
          {
            addComponentToNode(node, componentType);
          }
        }
        ImGui::EndMenu();
      }

      // Add node
      if (ImGui::MenuItem("Add Node"))
      {
        static crude::core::uint16 uniqueIndex = 0;
        flecs::entity newEntity = node.world().entity(std::format("Added Node {}", uniqueIndex++).c_str());
        newEntity.set<crude::scene::Transform>(crude::scene::Transform(newEntity));
        newEntity.child_of(node);
      }
      ImGui::EndPopup();
    }

    // DragDropSource
    if (ImGui::BeginDragDropSource())
    {
      ImGui::SetDragDropPayload(node.name(), NULL, 0);
      ImGui::Text(node.name());
      ImGui::EndDragDropSource();
    }

    ++currentNodeIndex;
    if (isTreeNodeOpen)
    {
      node.children(drawSceneNode);
      ImGui::TreePop();
    }
    };

  ImGui::Begin("Scene");
  drawSceneNode(layoutCtx->sceneNode);
  ImGui::End();
}

}