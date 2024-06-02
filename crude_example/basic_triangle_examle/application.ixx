export module application;

export import crude.engine;

export
{

class Application : public crude::Engine
{
public:
  void run() noexcept;
};

}