module crude.system.mouse_event_handler;

namespace crude::system
{

Mouse_Event_Handler::Mouse_Event_Handler()
  :
  m_leftIsDown(false),
  m_rightIsDown(false),
  m_middleDown(false)
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

void Mouse_Event_Handler::onLeftPressed(const Mouse_Point& position)
{
  m_leftIsDown = true;
  m_eventBuffer.push(Mouse_Event(MOUSE_EVENT_TYPE_LPRESS, position));
}

void Mouse_Event_Handler::onLeftReleased(const Mouse_Point& position)
{
  m_leftIsDown = false;
  m_eventBuffer.push(Mouse_Event(MOUSE_EVENT_TYPE_LRELEASE, position));
}

void Mouse_Event_Handler::onRightPressed(const Mouse_Point& position)
{
  m_rightIsDown = true;
  m_eventBuffer.push(Mouse_Event(MOUSE_EVENT_TYPE_RPRESS, position));
}

void Mouse_Event_Handler::onRightReleased(const Mouse_Point& position)
{
  m_rightIsDown = false;
  m_eventBuffer.push(Mouse_Event(MOUSE_EVENT_TYPE_RRELEASE, position));
}

void Mouse_Event_Handler::onMiddlePressed(const Mouse_Point& position)
{
  m_middleDown = true;
  m_eventBuffer.push(Mouse_Event(MOUSE_EVENT_TYPE_MPRESS, position));
}

void Mouse_Event_Handler::onMiddleReleased(const Mouse_Point& position)
{
  m_middleDown = false;
  m_eventBuffer.push(Mouse_Event(MOUSE_EVENT_TYPE_MRELEASE, position));
}

void Mouse_Event_Handler::onWheelUp(const Mouse_Point& position)
{
  m_eventBuffer.push(Mouse_Event(MOUSE_EVENT_TYPE_WHEELUP, position));
}

void Mouse_Event_Handler::onWheelDown(const Mouse_Point& position)
{
  m_eventBuffer.push(Mouse_Event(MOUSE_EVENT_TYPE_WHEELDOWN, position));
}

void Mouse_Event_Handler::onMouseMove(const Mouse_Point& position, const Mouse_Point& positionRel)
{
  m_position = position;
  m_eventBuffer.push(Mouse_Event(MOUSE_EVENT_TYPE_MOVE, position, positionRel));
}

void Mouse_Event_Handler::onMouseMoveRaw(const Mouse_Point& position)
{
  m_eventBuffer.push(Mouse_Event(MOUSE_EVENT_TYPE_RAW_MOVE, position));
}

}
