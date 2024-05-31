#include <SDL3/SDL.h>

module crude.system.sdl_io_manager;

namespace crude::system
{

void SDL_IO_Manager::update()
{
  SDL_Event e; 
  while (SDL_PollEvent(&e)) 
  { 
    if (e.type == SDL_EVENT_QUIT)
      m_windowEventHandler.m_eventBuffer.push(Window_Event(WINDOW_EVENT_TYPE_SHOULD_CLOSE)); 
  } 
}

}
