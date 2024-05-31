module crude.system.mouse_event_handler;

namespace crude::system
{

Mouse_Event_Handler::Mouse_Event_Handler()
  :
  m_leftIsDown(false),
  m_rightIsDown(false),
  m_middleDowm(false),
  m_x(0),
  m_y(0)
{}
  
Mouse_Event Mouse_Event_Handler::readEvent()
{
  if (m_eventBuffer.empty())
  {
    return Mouse_Event();
  }
  else
  {
    Mouse_Event event = m_eventBuffer.front();
    m_eventBuffer.pop();
    return event;
  }
}

bool Mouse_Event_Handler::eventBufferIsEmpty() const
{
  return m_eventBuffer.empty();
}

bool Mouse_Event_Handler::isLeftDown() const
{
  return m_leftIsDown;
}

bool Mouse_Event_Handler::isMiddleDown() const
{
  return m_middleDown;
}

bool Mouse_Event_Handler::isRightDown() const
{
  return m_rightIsDown;
}

core::int32 Mouse_Event_Handler::getPositionX() const
{
  return m_x;
}

core::int32 Mouse_Event_Handler::getPositionY() const
{
  return m_y;
}

const Mouse_Point& Mouse_Event_Handler::getPosition() const
{
  const Mouse_Point point = { m_x, m_y };
  return point;
}

void Mouse_Event_Handler::onLeftPressed(core::int32 x, core::int32 y)
{
  m_leftIsDown = true;
  m_eventBuffer.push(Mouse_Event(MOUSE_EVENT_TYPE_LPRESS, x, y));
}

void Mouse_Event_Handler::onLeftReleased(core::int32 x, core::int32 y)
{
  m_leftIsDown = false;
  m_eventBuffer.push(Mouse_Event(MOUSE_EVENT_TYPE_LRELEASE, x, y));
}

void Mouse_Event_Handler::onRightPressed(core::int32 x, core::int32 y)
{
  m_rightIsDown = true;
  m_eventBuffer.push(Mouse_Event(MOUSE_EVENT_TYPE_RPRESS, x, y));
}

void Mouse_Event_Handler::onRightReleased(core::int32 x, core::int32 y)
{
  m_rightIsDown = false;
  m_eventBuffer.push(Mouse_Event(MOUSE_EVENT_TYPE_RRELEASE, x, y));
}

void Mouse_Event_Handler::onMiddlePressed(core::int32 x, core::int32 y)
{
  m_middleDown = true;
  m_eventBuffer.push(Mouse_Event(MOUSE_EVENT_TYPE_MPRESS, x, y));
}

void Mouse_Event_Handler::onMiddleReleased(core::int32 x, core::int32 y)
{
  m_middleDown = false;
  m_eventBuffer.push(Mouse_Event(MOUSE_EVENT_TYPE_MRELEASE, x, y));
}

void Mouse_Event_Handler::onWheelUp(core::int32 x, core::int32 y)
{
  m_eventBuffer.push(Mouse_Event(MOUSE_EVENT_TYPE_WHEELUP, x, y));
}

void Mouse_Event_Handler::onWheelDown(core::int32 x, core::int32 y)
{
  m_eventBuffer.push(Mouse_Event(MOUSE_EVENT_TYPE_WHEELDOWN, x, y));
}

void Mouse_Event_Handler::onMouseMove(core::int32 x, core::int32 y)
{
  m_x = x;
  m_y = y;
  m_eventBuffer.push(Mouse_Event(MOUSE_EVENT_TYPE_MOVE, x, y));
}

void Mouse_Event_Handler::onMouseMoveRaw(core::int32 x, core::int32 y)
{
  m_eventBuffer.push(Mouse_Event(MOUSE_EVENT_TYPE_RAWMOVE, x, y));
}

}
