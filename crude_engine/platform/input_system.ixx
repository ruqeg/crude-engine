module;

#include <SDL3/SDL.h>
#include <flecs.h>
#include <functional>

export module crude.system.input_system;

export import crude.core.std_containers_heap;

export namespace crude::platform
{

struct Input_System_Component
{
  Input_System_Component() = default;
  Input_System_Component(std::function<void()> callback) : callback{callback}, inputEvent{} {}
  std::function<void()>  callback;
  SDL_Event              inputEvent;
};

void inputSystemProcess(flecs::iter& it)
{
  Input_System_Component* inputSystemComponent = it.world().get_mut<Input_System_Component>();

  while (SDL_PollEvent(&inputSystemComponent->inputEvent))
  {
    inputSystemComponent->callback();
  }
}

}
