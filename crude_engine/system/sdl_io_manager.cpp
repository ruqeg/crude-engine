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
    {
      m_windowEH.m_eventBuffer.push(Window_Event(WINDOW_EVENT_TYPE_SHOULD_CLOSE));
    }

    switch (e.type)
    {
    case SDL_EVENT_KEY_DOWN: 
      m_keyboardEH.onKeyPressed(e.key.keysym.scancode); 
      break;
    case SDL_EVENT_KEY_UP: 
      m_keyboardEH.onKeyReleased(e.key.keysym.scancode); 
      break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN: 
      switch (e.button.button)
      {
      case SDL_BUTTON_LEFT: 
        m_mouseEH.onLeftPressed(Mouse_Point(e.button.x, e.button.y));
        break;
      case SDL_BUTTON_RIGHT: 
        m_mouseEH.onRightPressed(Mouse_Point(e.button.x, e.button.y));
        break;
      }
      break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
      switch (e.button.button)
      {
      case SDL_BUTTON_LEFT:
        m_mouseEH.onLeftReleased(Mouse_Point(e.button.x, e.button.y));
        break;
      case SDL_BUTTON_RIGHT:
        m_mouseEH.onRightReleased(Mouse_Point(e.button.x, e.button.y));
        break;
      }
      break;
    case SDL_EVENT_MOUSE_MOTION:
      m_mouseEH.onMouseMove(Mouse_Point(e.button.x, e.button.y), Mouse_Point(e.motion.xrel, e.motion.yrel));
      break;
    }
  } 
}

}
