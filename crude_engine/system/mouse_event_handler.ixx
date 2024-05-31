module;

export module crude.system.mouse_event_handler;

export import crude.system.mouse_event;

export namespace crude::system 
{

class Mouse_Event_Handler
{
public:
  Mouse_Event_Handler();
public:
  Mouse_Event readEvent();
public:
  bool eventBufferIsEmpty() const;
  bool isLeftDown() const;
  bool isMiddleDown() const;
  bool isRightDown() const;
  core::int32 getPositionX() const;
  core::int32 getPositionY() const;
  const Mouse_Point& getPosition() const;
private:
  void onLeftPressed(core::int32 x, core::int32 y);
  void onLeftReleased(core::int32 x, core::int32 y);
  void onRightPressed(core::int32 x, core::int32 y);
  void onRightReleased(core::int32 x, core::int32 y);
  void onMiddlePressed(core::int32 x, core::int32 y);
  void onMiddleReleased(core::int32 x, core::int32 y);
  void onWheelUp(core::int32 x, core::int32 y);
  void onWheelDown(core::int32 x, core::int32 y);
  void onMouseMove(core::int32 x, core::int32 y);
  void onMouseMoveRaw(core::int32 x, core::int32 y);
private:
  core::queue<Mouse_Event>  m_eventBuffer;
  bool                      m_leftIsDown;
  bool                      m_rightIsDown;
  bool                      m_middleDown;
  core::int32               m_x;
  core::int32               m_y;
private:
  friend class SDL_IO_Manager;
};

}
