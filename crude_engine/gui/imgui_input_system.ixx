module;

#include <flecs.h>
#include <imgui/backends/imgui_impl_sdl3.h>

export module crude.gui.imgui_input_system;

import crude.platform.input_system;

export namespace crude::gui
{

void imguiUpdateEventSystemProcess(flecs::iter& it)
{
  const SDL_Event& inputEvent = it.world().get<platform::Input_System_Component>()->inputEvent;
  ImGui_ImplSDL3_ProcessEvent(&inputEvent);
}

}
