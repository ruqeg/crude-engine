module;

export module crude.system.sdl_system;

export namespace crude::system
{

class SDL_System
{
private:
  SDL_System() = default;
  ~SDL_System();
public:
  static SDL_System& getInstance();
public:
  bool initialize();
  bool initializeVulkan();
  void deinitializeVulkan();
  void deinitialize();
  bool isInitialized() const;
};

}
