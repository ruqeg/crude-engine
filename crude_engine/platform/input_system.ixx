module;

#include <SDL3/SDL.h>
#include <flecs.h>
#include <functional>
#include <imgui/backends/imgui_impl_sdl3.h>

export module crude.platform.input_system;

export import crude.core.std_containers_heap;

export namespace crude::platform
{

struct Input_System_Context
{
  core::vector<flecs::system>  inputSystems;
  SDL_Event                    inputEvent;
};

void inputSystemProcess(flecs::iter& it)
{
  Input_System_Context* inputSystemCtx = it.ctx<platform::Input_System_Context>();

  while (SDL_PollEvent(&inputSystemCtx->inputEvent))
  {
    for (auto& inputSystem : inputSystemCtx->inputSystems)
    {
      inputSystem.run();
    }
  }
}

}
