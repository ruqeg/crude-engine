module;

#include <SDL3/SDL.h>
#include <flecs.h>
#include <functional>

export module crude.system.input_system;

export import crude.core.std_containers_heap;

export namespace crude::system
{

struct Update_Event_System_Context
{
  SDL_Event inputEvent;
};

struct Input_System_Context
{
  std::function<void(core::shared_ptr<const Update_Event_System_Context>)> callback;
};

void inputSystem(flecs::iter& it, size_t index)
{
  auto inputSystemCtx = *it.ctx<core::shared_ptr<const Input_System_Context>>();

  SDL_Event inputEvent;
  while (SDL_PollEvent(&inputEvent))
  {
    auto updateEventCtx = core::allocateShared<Update_Event_System_Context>();
    updateEventCtx->inputEvent = inputEvent;
    inputSystemCtx->callback(updateEventCtx);
  }
}

}
