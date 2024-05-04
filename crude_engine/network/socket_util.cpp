#include <network/socket_util.hpp>
#include <network/udp_socket.hpp>
#include <network/tcp_socket.hpp>

namespace crude_engine
{

Shared_Ptr<UDP_Socket> Socket_Util::createUDPSocket(Socket_Address_Family inFamily)
{
  SOCKET s = socket(inFamily, SOCK_DGRAM, IPPROTO_UDP);
  if (s == INVALID_SOCKET)
  {
    CRUDE_ASSERT("TODO" && false);
    return nullptr;
  }
  return makeShared<UDP_Socket>(s);
}

Shared_Ptr<TCP_Socket> Socket_Util::createTCPSocket(Socket_Address_Family inFamily)
{
  SOCKET s = socket(inFamily, SOCK_STREAM, IPPROTO_TCP);
  if (s == INVALID_SOCKET)
  {
    CRUDE_ASSERT("TODO" && false);
    return nullptr;
  }
  return makeShared<TCP_Socket>(s);
}

}