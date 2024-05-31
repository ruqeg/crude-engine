module;

export module crude.system.window_event_handler;

export import crude.system.window_event;

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
};

}
