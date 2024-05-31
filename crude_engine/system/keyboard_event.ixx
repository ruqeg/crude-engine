module;

export module crude.system.keyboard_event;

import crude.core.alias;

export namespace crude::system
{

enum Keyboard_Event_Type
{
  KEYBOARD_EVENT_TYPE_PRESS,
  KEYBOARD_EVENT_TYPE_RELEASE,
  KEYBOARD_EVENT_TYPE_INVALID
};

class Keyboard_Event
{
public:
  Keyboard_Event()
    : m_type(KEYBOARD_EVENT_TYPE_INVALID), m_key(0) {}
  Keyboard_Event(const Keyboard_Event_Type type, const core::uint8 key)
    : m_type(type), m_key(key) {}
  bool isPress() const { return m_type == KEYBOARD_EVENT_TYPE_PRESS; }
  bool isRelease() const { return m_type == KEYBOARD_EVENT_TYPE_RELEASE; }
  bool isValid() const { return m_type != KEYBOARD_EVENT_TYPE_INVALID; }
  core::uint8 getKeyCode() const { return m_key; }

private:
  Keyboard_Event_Type m_type;
  core::uint8         m_key;
};

}
