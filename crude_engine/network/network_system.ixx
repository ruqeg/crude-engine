module;

export module crude.network.network_system;

export namespace crude::network
{

class Network_System 
{
public:
  static Network_System& getInstance();
public:
  void initialize();
  void deinitialize();
private:
  Network_System() = default;
  ~Network_System();
};

}
