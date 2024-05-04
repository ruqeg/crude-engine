#pragma once

#include <core/shared_ptr.hpp>

#include <winsock2.h>
#include <ws2tcpip.h>

namespace crude_engine
{

enum Socket_Address_Family
{
  SOCKET_ADDRESS_FAMILT_INET  = AF_INET,
  SOCKET_ADDRESS_FAMILT_INET6 = AF_INET6
};

class UDP_Socket;
class TCP_Socket;

class Socket_Util
{
public:
  static Shared_Ptr<UDP_Socket> createUDPSocket(Socket_Address_Family inFamily);
  static Shared_Ptr<TCP_Socket> createTCPSocket(Socket_Address_Family inFamily);
};

}