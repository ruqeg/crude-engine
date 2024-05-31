module;

export module crude.system.window_event_handler;

export import crude.system.window_event;
import crude.core.std_containers_heap;

export namespace crude::system
{

class Window_Event_Handler
{
public:
  Window_Event_Handler() = default;
public:
  Window_Event readEvent();
public:
  bool eventBufferIsEmpty() const;
private:
  core::queue<Window_Event> m_eventBuffer;

  friend class SDL_IO_Manager;
};

}
