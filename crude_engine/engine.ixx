module;

#include <flecs.h>

export module crude.engine;

export import crude.core.timer;


// !TODO
import crude.core.std_containers_heap;
export namespace crude::graphics
{
class Render_Pass;
class Descriptor_Pool;
class Framebuffer;
class Command_Buffer;
}

export namespace crude
{

// !TODO
class Engine
{
public:
  static void preinitialize(core::uint32 defaultFreeRBTCapacity);
  static void postdeinitialize();
public:
  void initialize(const char* title, core::uint32 width, core::uint32 height);
  void deinitialize();
  void mainLoop();
private:
  void registerSystems();
  void update(core::float64 elapsed);
  void render();
private:
  core::Timer    m_timer;
  flecs::world   m_world;
  flecs::system  m_freeCameraUpdateSystem;
  flecs::system  m_inputSystem;
  flecs::system  m_rendererFrameStartSystem;
  flecs::system  m_deferredGBufferPassSystem;
  flecs::system  m_fullscreenPBRPassSystem;
  flecs::system  m_imguiRendererPassSystem;
  flecs::system  m_rendererFrameSubmitSystem;
  flecs::entity  m_sceneNode;
};

}