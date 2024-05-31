module;

export module crude.system.window_event;

export namespace crude::system
{

enum Window_Event_Type
{
  WINDOW_EVENT_TYPE_SHOULD_CLOSE,
  WINDOW_EVENT_TYPE_INVALID,
};

class Window_Event
{
public:
  Window_Event()
    : m_type(WINDOW_EVENT_TYPE_INVALID) {}
  Window_Event(const Window_Event_Type type)
    : m_type(type) {}
public:
  bool isValid() const { return m_type != WINDOW_EVENT_TYPE_INVALID; };
  Window_Event_Type getType() { return m_type; };
  bool shouldClose() { return m_type == WINDOW_EVENT_TYPE_SHOULD_CLOSE; }
private:
  Window_Event_Type m_type;
};

}
