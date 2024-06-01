#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

module crude.network.network_system;

namespace crude::network 
{

Network_System& Network_System::getInstance()
{
  static Network_System instance;
  return instance;
}

void Network_System::initialize()
{
  WSADATA init;
  WSAStartup(MAKEWORD(2, 2), &init);
}

void Network_System::deinitialize()
{
  WSACleanup();
}

Network_System::~Network_System()
{
  deinitialize();
}

}
