module;

export module crude.system.windows_event;

export namespace crude::system
{

enum Window_Event_Type
{
  WINDOW_EVENT_TYPE_RESIZED,
  WINDOW_EVENT_TYPE_INVALID,
};

class Window_Event
{
public:
  Window_Event()
    : m_type(0) {}
  Window_Event(const Window_Event_Type type)
    : m_type(type) {}
public:
  bool isValid() const { return m_type != WINDOW_EVENT_TYPE_INVALID; };
  Window_Event_Type getType() { return m_type; };
private:
  Window_Event_Type m_type;
};

}
