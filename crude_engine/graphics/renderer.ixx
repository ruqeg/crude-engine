module;

export module crude.graphics.renderer;

export import crude.core.alias;
export import crude.core.shared_ptr;
export import crude.graphics.queue;
export import crude.graphics.device;
export import crude.graphics.physical_device;
export import crude.graphics.instance;
export import crude.graphics.surface;
export import crude.graphics.debug_utils_messenger;
export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.system.sdl_window_container;

export namespace crude::graphics
{

class Renderer
{
private:
  struct Queue_Family_Indices
  {
    core::Optional<core::uint32> graphicsFamily;
    core::Optional<core::uint32> presentFamily;

    bool isComplete() const
    {
      return graphicsFamily.hasValue() && presentFamily.hasValue();
    }
  };
public:
  Renderer(core::Shared_Ptr<system::SDL_Window_Container> windowContainer);
private:
  void initializeInstance();
  void initializeSurface();
  void initializeDevice();
  core::Shared_Ptr<Physical_Device> pickPhysicalDevice();
  Queue_Family_Indices findDeviceQueueFamilies(core::Shared_Ptr<Physical_Device> physicalDevice);
  bool checkDeviceExtensionSupport(core::Shared_Ptr<Physical_Device> physicalDevice);
  bool checkSurfaceSupport(core::Shared_Ptr<Physical_Device> physicalDevice, core::Shared_Ptr<Surface> surface);
  void initializeLogicDevice(core::Shared_Ptr<Physical_Device> physicalDevice);
private:
  core::Shared_Ptr<Instance>                       m_instance;
  core::Shared_Ptr<Device>                         m_device;
  core::Shared_Ptr<Queue>  m_graphicsQueue;
  core::Shared_Ptr<Queue>  m_presentQueue;
  core::Shared_Ptr<Surface>                        m_surface;
  core::Shared_Ptr<Debug_Utils_Messenger>          m_debugUtilsMessenger;
  core::Shared_Ptr<system::SDL_Window_Container>   m_windowContainer;
};

}