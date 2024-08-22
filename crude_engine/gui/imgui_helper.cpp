#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/backends/imgui_impl_vulkan.h>

module crude.gui.imgui_helper;

import crude.platform.sdl_window_container;

namespace crude::gui
{

void initializeImGuiContext()
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui::StyleColorsDark();
}

void initializeImGuiVulkanSDL(core::shared_ptr<platform::SDL_Window_Container> windowContainer)
{
  ImGui_ImplSDL3_InitForVulkan(windowContainer->getHandle().get());
}

void deinitializeImGuiVulkanSDL()
{
  ImGui_ImplSDL3_Shutdown();
}

void deinitializeImGuiContext()
{
  ImGui::DestroyContext();
}

}
