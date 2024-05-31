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

bool SDL_Window_Container::isValid() const
{
  return (m_window.get() != nullptr);
}

std::shared_ptr<const SDL_Window> SDL_Window_Container::getWindow() const
{
  return m_window;
}

std::shared_ptr<SDL_Window> SDL_Window_Container::getWindow()
{
  return m_window;
}

}
