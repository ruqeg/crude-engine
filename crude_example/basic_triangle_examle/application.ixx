module;

export module application;

export import crude.engine;

export
{

class Application : public crude::Engine
{
public:
  Application(crude::core::shared_ptr<crude::system::SDL_Window_Container> windowContainer);
  void run() noexcept;
};

}