#include <flecs.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/backends/imgui_impl_vulkan.h>

module gui.imgui_editor_layout_draw_system;

import crude.graphics.texture;
import crude.gui.imgui_texture_descriptor_set;

namespace gui
{

void imguiEditorLayoutDrawSystemProcess(flecs::iter& it)
{
  Imgui_Editor_Layout_Draw_Ctx* editorLayoutCtx = it.ctx<Imgui_Editor_Layout_Draw_Ctx>();

  ImGui::Begin("Viewport");
  ImGui::Text("RED BALL SOON 0_0 !!!!!!!!!!!!");
  ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
  ImGui::Image(editorLayoutCtx->sceneImguiTextureDescriptorSet->getHandle(), ImVec2{viewportPanelSize.x, viewportPanelSize.y});
  ImGui::End();
}

}