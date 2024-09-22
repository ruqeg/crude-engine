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
class Renderer_Fullscreen_PBR_Pass_Ctx;
}

export namespace crude::gui
{
class ImGui_Texture_Descriptor_Set;
}

export namespace crude::editor::gui
{

struct Imgui_Editor_Layout_Draw_Ctx
{
  core::shared_ptr<crude::gui::ImGui_Texture_Descriptor_Set>    viewportImguiTexture;
  core::shared_ptr<graphics::Renderer_Fullscreen_PBR_Pass_Ctx>  rendererFullscreenPbrPass;
  flecs::entity                                                 sceneNode;
  flecs::entity                                                 editorCameraNode;
  flecs::entity                                                 editorSelectedNode;
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