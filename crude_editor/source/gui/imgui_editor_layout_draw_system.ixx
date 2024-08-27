module;

#include <flecs.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/backends/imgui_impl_vulkan.h>

export module gui.imgui_editor_layout_draw_system;

export import crude.core.std_containers_heap;

export namespace crude::graphics
{
class Texture;
}

export namespace gui
{

struct Imgui_Editor_Layout_Draw_Ctx
{
  crude::core::shared_ptr<crude::graphics::Texture> sceneTexture;
};

void imguiEditorLayoutDrawSystemProcess(flecs::iter& it);

}