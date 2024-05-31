module crude.system.window_event_handler;

namespace crude::system 
{

Window_Event Window_Event_Handler::readEvent()
{
  if (m_eventBuffer.empty())
  {
    return Window_Event();
  }
  else
  {
    Window_Event event = m_eventBuffer.front();
    m_eventBuffer.pop();
    return event;
  }
}

bool Window_Event_Handler::eventBufferIsEmpty() const
{
  return m_eventBuffer.empty();
}

}
