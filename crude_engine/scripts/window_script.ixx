module;

#include <flecs.h>
#include <SDL3/SDL.h>

export module crude.scripts.window_script;

export import crude.core.std_containers_heap;
import crude.platform.input_system;

export namespace crude::scripts
{

struct Window_Script_Component
{
  bool shouldClose;
};

void windowScriptInputSystemProcess(flecs::iter& it)
{
  const SDL_Event& inputEvent = it.ctx<platform::Input_System_Context>()->inputEvent;
  if (inputEvent.type == SDL_EVENT_QUIT)
  {
    auto* windowComponent = it.world().get_mut<Window_Script_Component>();
    windowComponent->shouldClose = true;
  }
}

}
