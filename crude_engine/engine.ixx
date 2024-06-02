module;

export module crude.engine;

import crude.system.sdl_system;
import crude.core.memory_manager;
import crude.graphics.renderer;
import crude.network.network_system;
import crude.core.alias;

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
  Engine();
public:
  static void initialize(const Engine_Config& config);
private:
  static void initializeMemory(core::uint32 defaultFreeRBTCapacity);
  static void initalizeSystem();
  static void initalizeNetwork();
private:
  core::Shared_Ptr<system::SDL_Window_Container>  m_windowContainer;
  core::Shared_Ptr<graphics::Renderer>            m_renderer;
  
};

}