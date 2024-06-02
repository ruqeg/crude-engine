module crude.engine;

namespace crude
{

Engine::Engine()
{
  m_windowContainer = core::makeShared<system::SDL_Window_Container>("TEST", 600, 600, system::SDL_WINDOW_CONTAINER_FLAG_VULKAN);
  m_renderer = core::makeShared<graphics::Renderer>(m_windowContainer);
}
void Engine::initialize(const Engine_Config& config)
{
  initializeMemory(config.defaultFreeRBTCapacity);
  initalizeSystem();
  initalizeNetwork();
}

void Engine::initializeMemory(core::uint32 defaultFreeRBTCapacity)
{
  core::Memory_Manager::getInstance().initialize(defaultFreeRBTCapacity);
}

void Engine::initalizeSystem()
{
  crude::system::SDL_System::getInstance().initialize();
  crude::system::SDL_System::getInstance().initializeVulkan();
}

void Engine::initalizeNetwork()
{
  crude::network::Network_System::getInstance().initialize();
}

}