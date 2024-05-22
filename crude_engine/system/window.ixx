module;

#include <SDL3/SDL.h>

export module crude.system.windows;

import crude.core.alias;

export namespace crude::system
{

class Window
{
public:
  Window();
  ~Window();

public:
  bool create(const char* title, core::int32 width, core::int32 height, core::uint32 flags);

private:
  SDL_Window* m_window;
};

}