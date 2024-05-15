module;

#include <winsock2.h>
#include <ws2tcpip.h>

export module crude_engine.network.socket_address_factory;

import crude_engine.network.socket_address;

export namespace crude_engine
{

class Socket_Address_Factory
{
public:
  static Socket_Address_Ptr createIPv4FromString(const char* inString);
};

}