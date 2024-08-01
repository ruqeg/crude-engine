#include <SDL3/SDL.h>

module crude.system.sdl_window_container;

namespace crude::system
{

SDL_Window_Container::SDL_Window_Container(const char* title, core::int32 width, core::int32 height, core::uint32 flags)
{
  m_window = std::shared_ptr<SDL_Window>(
    SDL_CreateWindow(title, width, height, flags),
    SDL_DestroyWindow);
}

core::uint32 SDL_Window_Container::getWidth() const
{
  int width, height;
  SDL_GetWindowSize(m_window.get(), &width, &height);
  return width;
}

core::uint32 SDL_Window_Container::getHeight() const
{
  int width, height;
  SDL_GetWindowSize(m_window.get(), &width, &height);
  return height;
}

core::float32 SDL_Window_Container::getAspect() const
{
  int width, height;
  SDL_GetWindowSize(m_window.get(), &width, &height);
  return static_cast<core::float32>(width) / height;
}

}
