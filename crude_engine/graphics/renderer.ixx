module;

export module crude.graphics.renderer;

export import crude.core.alias;
export import crude.core.shared_ptr;
export import crude.graphics.instance;
export import crude.graphics.surface;
export import crude.graphics.debug_utils_messenger;
export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;

export namespace crude::graphics
{

class Renderer
{
public:
  using Instace_Ptr               = core::Shared_Ptr<Instance>;
  using Debug_Utils_Messenger_Ptr = core::Shared_Ptr<Debug_Utils_Messenger>;
public:
  Renderer();
private:
  void initializeInstance();
private:
  Instace_Ptr                m_instance;
  Debug_Utils_Messenger_Ptr  m_debugUtilsMessenger;
};

}