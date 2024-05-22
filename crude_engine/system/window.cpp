#include <SDL3/SDL.h>

module crude.system.windows;

namespace crude::system
{

Window::Window()
  :
  m_window(nullptr)
{}

Window::~Window()
{
  if (m_window)
  {
    SDL_DestroyWindow(m_window);
  }
}

bool Window::create(const char* title, core::int32 width, core::int32 height, core::uint32 flags)
{
  m_window = SDL_CreateWindow(title, width, height, flags);
  return (m_window != nullptr);
}

}
