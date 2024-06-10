module;

export module crude.system.mouse_event;

import crude.core.alias;
import crude.math.fuicont;

export namespace crude::system
{

struct Mouse_Point : public math::Float2 
{
  Mouse_Point() = default;
  constexpr Mouse_Point(core::float32 x, core::float32 y) noexcept 
    : math::Float2(x, y) {}
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
    : m_type(MOUSE_EVENT_TYPE_INVALID) {}
  Mouse_Event(const Mouse_Event_Type type, const core::float32 x, const core::float32 y, const core::float32 xrel = 0.0f, const core::float32 yrel = 0.0f)
    : m_type(type), m_position(x, y), m_positionRel(xrel, yrel) {}
  Mouse_Event(const Mouse_Event_Type type, const Mouse_Point& position, const Mouse_Point& positionRel = {})
    : m_type(type), m_position(position), m_positionRel(positionRel) {}
  bool isVaild() const { return m_type != MOUSE_EVENT_TYPE_INVALID; }
  Mouse_Event_Type getType() const { return m_type; };
  const Mouse_Point& getPosition() const { return m_position; }
  core::float32 getPositionX() const { return m_position.x; }
  core::float32 getPositionY() const { return m_position.y; }
  const Mouse_Point& getPositionRel() const { return m_positionRel; }
  core::float32 getPositionRelX() const { return m_positionRel.x; }
  core::float32 getPositionRelY() const { return m_positionRel.y; }
private:
  Mouse_Event_Type  m_type;
  Mouse_Point       m_position;
  Mouse_Point       m_positionRel;
};

}
