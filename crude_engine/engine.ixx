module;

export module crude.engine;

export import crude.system.sdl_system;
export import crude.core.memory_manager;
export import crude.graphics.renderer;
export import crude.network.network_system;
export import crude.system.sdl_io_manager;

export namespace crude
{

struct Engine_Config
{
  core::uint32 defaultFreeRBTCapacity;
};

// !TODO
class Engine
{
public:
  Engine(core::Shared_Ptr<system::SDL_Window_Container> windowContainer);
public:
  static void initialize(const Engine_Config& config);
private:
  static void initializeMemory(core::uint32 defaultFreeRBTCapacity);
  static void initalizeSystem();
  static void initalizeNetwork();
protected:
  graphics::Renderer      m_renderer;
  system::SDL_IO_Manager  m_ioManager;
  
};

}