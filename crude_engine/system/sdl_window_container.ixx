module;

#include <SDL3/SDL.h>
#include <memory>

export module crude.system.sdl_window_container;

import crude.core.alias;

export namespace crude::system
{

enum SDL_Window_Container_Flag
{
  SDL_WINDOW_CONTAINER_FLAG_VULKAN = SDL_WINDOW_VULKAN
};

class SDL_Window_Container
{
public:
  SDL_Window_Container(const char* title, core::int32 width, core::int32 height, core::uint32 flags);
  ~SDL_Window_Container() = default;

  bool isValid() const;
  std::shared_ptr<SDL_Window> getWindow();
  std::shared_ptr<const SDL_Window> getWindow() const;

private:
  std::shared_ptr<SDL_Window> m_window;
};

}
