module;

#include <flecs.h>

export module crude.engine;

export import crude.core.timer;

export namespace crude
{

struct Engine_Initialize
{
  core::uint32 defaultFreeRBTCapacity;
  core::uint32 width;
  core::uint32 height;
  const char*  title;
};

// !TODO
class Engine
{
public:
  void initialize(const Engine_Initialize& config);
  void deinitialize();
  void mainLoop();
private:
  void initializeSystems();
private:
  core::Timer    m_timer;
  flecs::world   m_world;
  flecs::system  m_freeCameraUpdateSystem;
  flecs::system  m_inputSystem;
  flecs::system  m_deferredGBufferPassSystem;
  flecs::system  m_fullscreenPBRPassSystem;
  flecs::system  m_rendererFrameStartSystem;
  flecs::system  m_rendererFrameSubmitSystem;
  flecs::entity  m_sceneNode;
};

}