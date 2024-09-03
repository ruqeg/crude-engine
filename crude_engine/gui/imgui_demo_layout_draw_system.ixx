module;

#include <flecs.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/backends/imgui_impl_vulkan.h>

export module crude.gui.imgui_demo_layout_draw_system;

export namespace crude::gui
{

void imguiDemoLayoutDrawSystemProcess(flecs::iter& it)
{
  // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
  static bool show_demo_window = true;
  if (show_demo_window)
    ImGui::ShowDemoWindow(&show_demo_window);

  // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
  {
    ImGui::Begin("imguiDemoSettings");
    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    ImGui::End();
  }
}

}