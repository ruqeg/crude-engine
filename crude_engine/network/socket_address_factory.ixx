module;

#include <winsock2.h>
#include <ws2tcpip.h>

export module crude.network.socket_address_factory;

import crude.network.socket_address;

export namespace crude::network
{

class Socket_Address_Factory
{
public:
  static Socket_Address_Ptr createIPv4FromString(const char* inString);
};

}