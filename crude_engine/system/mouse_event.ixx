module;

export module crude.system.mouse_event;

import crude.core.alias;

export namespace crude::system
{

struct Mouse_Point
{
  core::int32 x;
  core::int32 y;
};
	
enum Mouse_Event_Type
{
  MOUSE_EVENT_TYPE_LPRESS,
  MOUSE_EVENT_TYPE_LRELEASE,
  MOUSE_EVENT_TYPE_RPRESS,
  MOUSE_EVENT_TYPE_RRELEASE,
  MOUSE_EVENT_TYPE_MPRESS,
  MOUSE_EVENT_TYPE_MRELEASE,
  MOUSE_EVENT_TYPE_WHEELUP,
  MOUSE_EVENT_TYPE_WHEELDOWN,
  MOUSE_EVENT_TYPE_MOVE,
  MOUSE_EVENT_TYPE_RAW_MOVE,
  MOUSE_EVENT_TYPE_INVALID,
};

class Mouse_Event
{
public:
  Mouse_Event()
    : m_type(MOUSE_EVENT_TYPE_INVALID), m_x(0), m_y(0) {}
  Mouse_Event(const Mouse_Event_Type type, const core::int32 x, const core::int32 y)
    : m_type(type), m_x(x), m_y(y) {}
  bool isVaild() const { return m_type != MOUSE_EVENT_TYPE_INVALID; }
  Mouse_Event_Type getType() const { return m_type; };
  Mouse_Point getPosition() const { return {m_x, m_y}; }
  core::int32 getPositionX() const { return m_x; }
  core::int32 getPositionY() const { return m_y; }

private:
  Mouse_Event_Type  m_type;
  core::int32       m_x;
  core::int32       m_y;
};

}
