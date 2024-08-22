module;

#include <SDL3/SDL.h>
#include <flecs.h>
#include <functional>
#include <imgui/backends/imgui_impl_sdl3.h>

export module crude.platform.input_system;

export import crude.core.std_containers_heap;

export namespace crude::platform
{

struct Input_System_Component
{
  Input_System_Component() = default;
  Input_System_Component(const core::vector<flecs::system>& eventSystems) : eventSystems(eventSystems) {}

  core::vector<flecs::system>  eventSystems;
  SDL_Event                    inputEvent;
};

void inputSystemProcess(flecs::iter& it)
{
  Input_System_Component* inputSystemComponent = it.world().get_mut<Input_System_Component>();

  while (SDL_PollEvent(&inputSystemComponent->inputEvent))
  {
    for (auto& eventSystem : inputSystemComponent->eventSystems)
    {
      eventSystem.run();
    }
  }
}

}
