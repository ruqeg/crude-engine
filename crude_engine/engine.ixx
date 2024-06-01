module;

export module crude.engine;

import crude.core.memory_manager;
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
};

}