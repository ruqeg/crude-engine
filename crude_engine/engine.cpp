module crude.engine;

namespace crude
{

Engine::Engine(const Engine_Config& config)
{
  core::Memory_Manager::getInstance().initialize(config.defaultFreeRBTCapacity);
}

}