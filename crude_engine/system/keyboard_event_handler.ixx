module;

export module crude.system.keyboard_event_handler;

export import crude.system.keyboard_event;

import crude.core.alias;
import crude.core.std_container_heap;
import crude.core.std_container_stack;

export namespace crude::system
{

class Keyboard_Event_Handler
{
public:
  Keyboard_Event_Handler();
  bool keyIsPressed(const core::uint8 keycode) const;
  bool keyBufferIsEmpty() const;
  bool charBufferIsEmpty() const;
  Keyboard_Event readKey();
  core::uint8 readChar();
  void enableAutoRepeatKeys();
  void disableAutoRepeatKeys();
  void enableAutoRepeatChars();
  void disableAutoRepeatChars();
  bool isKeysAutoRepeat() const;
  bool isCharsAutoRepeat() const;
private:
  void onKeyPressed(const core::uint8 key);
  void onKeyRelease(const core::uint8 key);
  void onChar(const core::uint8 key);
private:
  bool                         m_autoRepeatKeys;
  bool                         m_autoRepeatChars;
  core::array<bool, 256>       m_keyState;
  core::queue<Keyboard_Event>  m_keyBuffer;
  core::queue<core::uint8>     m_charBuffer;  
private:
  friend class SDL_IO_Manager;
};

}
