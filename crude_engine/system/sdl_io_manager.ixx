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
  Keyboard_Event_Handler& getKeyboardEH() { return m_keyboardEH; }
  Mouse_Event_Handler& getMouseEH() { return m_mouseEH; }
  Window_Event_Handler& getWindowEH() { return m_windowEH; }
private:
  Keyboard_Event_Handler  m_keyboardEH;
  Mouse_Event_Handler     m_mouseEH;
  Window_Event_Handler    m_windowEH;
};

}
