module;

#include <flecs.h>

export module application;

export import crude.engine;

export
{

class Application : public crude::Engine
{
public:
  Application();
  ~Application();
  void run();
};

}