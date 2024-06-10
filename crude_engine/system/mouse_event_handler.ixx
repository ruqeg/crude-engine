module;

export module crude.system.mouse_event_handler;

export import crude.system.mouse_event;

import crude.core.std_containers_heap;
import crude.core.alias;

export namespace crude::system 
{

class Mouse_Event_Handler
{
public:
  Mouse_Event_Handler();
public:
  Mouse_Event readEvent();
public:
  bool eventBufferIsEmpty() const { return m_eventBuffer.empty(); }
  bool isLeftDown() const { return m_leftIsDown; }
  bool isMiddleDown() const { return m_middleDown; }
  bool isRightDown() const { return m_rightIsDown; }
  core::float32 getPositionX() const { return m_position.x; }
  core::float32 getPositionY() const { return m_position.y; }
  const Mouse_Point& getPosition() const { return m_position; }
private:
  void onLeftPressed(const Mouse_Point& position);
  void onLeftReleased(const Mouse_Point& position);
  void onRightPressed(const Mouse_Point& position);
  void onRightReleased(const Mouse_Point& position);
  void onMiddlePressed(const Mouse_Point& position);
  void onMiddleReleased(const Mouse_Point& position);
  void onWheelUp(const Mouse_Point& position);
  void onWheelDown(const Mouse_Point& position);
  void onMouseMove(const Mouse_Point& position, const Mouse_Point& positionRel);
  void onMouseMoveRaw(const Mouse_Point& position);
private:
  core::queue<Mouse_Event>  m_eventBuffer;
  bool                      m_leftIsDown;
  bool                      m_rightIsDown;
  bool                      m_middleDown;
  Mouse_Point               m_position;
private:
  friend class SDL_IO_Manager;
};

}
