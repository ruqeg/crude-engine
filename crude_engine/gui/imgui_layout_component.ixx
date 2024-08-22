module;

#include <flecs.h>

export module crude.gui.imgui_layout_component;

export import crude.core.std_containers_heap;

export namespace crude::gui
{

struct ImGui_Layout_Component
{
  core::vector<flecs::system> drawLayoutSystems;
};

}