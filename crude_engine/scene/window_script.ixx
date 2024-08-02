module;

#include <flecs.h>
#include <SDL3/SDL.h>

export module crude.scene.window_script;

export import crude.core.std_containers_heap;
import crude.platform.input_system;

export namespace crude::scene::script
{

struct Window_Component
{
  bool shouldClose;
};

void windowUpdateEventSystemProcess(flecs::iter& it)
{
  const auto inputSystemComponent = it.world().get<platform::Input_System_Component>();
  const SDL_Event& inputEvent = inputSystemComponent->inputEvent;
  if (inputEvent.type == SDL_EVENT_QUIT)
  {
    auto* windowComponent = it.world().get_mut<Window_Component>();
    windowComponent->shouldClose = true;
  }
}

}
