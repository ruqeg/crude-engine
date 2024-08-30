module;

#include <vulkan/vulkan.h>
#include <flecs.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/backends/imgui_impl_vulkan.h>

export module gui.imgui_editor_layout_draw_system;

export import crude.core.std_containers_heap;

export namespace crude::gui
{
class ImGui_Texture_Descriptor_Set;
}

export namespace gui
{

struct Imgui_Editor_Layout_Draw_Ctx
{
  crude::core::shared_ptr<crude::gui::ImGui_Texture_Descriptor_Set> sceneImguiTextureDescriptorSet;
};

void imguiEditorLayoutDrawSystemProcess(flecs::iter& it);

}