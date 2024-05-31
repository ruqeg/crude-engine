module;

export module crude.system.sdl_io_manager;

export import crude.system.keyboard_event_handler;
export import crude.system.mouse_event_handler;
export import crude.system.window_event_handler;

export namespace crude::system
{

class SDL_IO_Manager 
{
public:
  void update();
private:
  Keyboard_Event_Handler  m_keyboardEventHandler;
  Mouse_Event_Handler     m_mouseEventHandler;
  Window_Event_Handler    m_windowEventHandler;
};

}
