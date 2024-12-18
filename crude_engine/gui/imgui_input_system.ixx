module;

#include <flecs.h>
#include <imgui/backends/imgui_impl_sdl3.h>

export module crude.gui.imgui_input_system;

import crude.platform.input_system;

export namespace crude::gui
{

void imguiInputSystemProcess(flecs::iter& it)
{
  const SDL_Event& inputEvent = it.ctx<platform::Input_System_Context>()->inputEvent;
  //ImGui_ImplSDL3_ProcessEvent(&inputEvent);
}

}
