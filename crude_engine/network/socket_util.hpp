#pragma once

#include <network/udp_socket.hpp>
#include <core/shared_ptr.hpp>

namespace crude_engine
{

enum Socket_Address_Family
{
  SOCKET_ADDRESS_FAMILT_INET  = AF_INET,
  SOCKET_ADDRESS_FAMILT_INET6 = AF_INET6
};

class Socket_Util
{
public:
  static Shared_Ptr<UDP_Socket> createUDPSocket(Socket_Address_Family inFamily);
};

}