module;

#include <vulkan/vulkan.h>
#include <flecs.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <functional>

export module crude.editor.gui.imgui_editor_layout_draw_system;

export import crude.core.std_containers_heap;

export namespace crude::graphics
{
class Command_Pool;
}

export namespace crude::gui
{
class ImGui_Texture_Descriptor_Set;
}

export namespace crude::editor::gui
{

struct Imgui_Editor_Layout_Draw_Ctx
{
  core::shared_ptr<crude::gui::ImGui_Texture_Descriptor_Set> viewportImguiTexture;
  flecs::entity                                              sceneNode;
  flecs::entity                                              editorCameraNode;
  flecs::entity                                              editorSelectedNode;
  std::function<void(flecs::entity)>                         sceneLoadedCallback;
  core::shared_ptr< crude::graphics::Command_Pool>           transferCommandPool;
};

void imguiEditorLayoutDrawSystemProcess(flecs::iter& it);

}

namespace crude::editor::gui
{
void preDraw(Imgui_Editor_Layout_Draw_Ctx* layoutCtx);
void drawMainMenuBar(Imgui_Editor_Layout_Draw_Ctx* layoutCtx);
void drawInspectorWindow(flecs::entity editorSelectedNode);
void drawViewportWindow(Imgui_Editor_Layout_Draw_Ctx* layoutCtx);
void drawSceneWindow(Imgui_Editor_Layout_Draw_Ctx* layoutCtx);
}