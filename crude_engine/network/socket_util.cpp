#include <network/socket_util.hpp>

namespace crude_engine
{

Shared_Ptr<UDP_Socket> Socket_Util::createUDPSocket(Socket_Address_Family inFamily)
{
  SOCKET s = socket(inFamily, SOCK_DGRAM, IPPROTO_UDP);
  if (s != INVALID_SOCKET)
  {
    return makeShared<UDP_Socket>(s);
  }
  else
  {
    CRUDE_ASSERT("TODO" && false);
    return nullptr;
  }
}

}