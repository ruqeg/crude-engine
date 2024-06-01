module;

export module crude.engine;

import crude.system.sdl_system;
import crude.core.memory_manager;
import crude.network.network_system;
import crude.core.alias;

export namespace crude
{

struct Engine_Config
{
  core::uint32 defaultFreeRBTCapacity;
};

class Engine
{
public:
  Engine(const Engine_Config& config);
  ~Engine() = default;
private:
  void initializeMemory(core::uint32 defaultFreeRBTCapacity);
  void initalizeSystem();
  void initalizeNetwork();
};

}