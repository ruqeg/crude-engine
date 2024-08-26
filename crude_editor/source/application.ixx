module;

#include <flecs.h>

export module application;

export import crude.engine;

export
{

class Application : public crude::Engine
{
public:
  void initialize();
  void run();
  void deinitialize();
private:
  void initializeScene(crude::core::float32 aspectRatio);
  void initializeCamera(crude::core::float32 aspectRatio);
private:
  flecs::system  m_freeCameraUpdateSystem;
};

}