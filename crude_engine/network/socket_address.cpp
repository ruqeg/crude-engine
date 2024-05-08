#include <winsock2.h>
#include <ws2tcpip.h>

module crude_engine.network.socket_address;

namespace crude_engine
{

Socket_Address::Socket_Address(uint32 inAddress, uint16 inPort)
{
  getAsSockAddrIn()->sin_family = AF_INET;
  getAsSockAddrIn()->sin_port = htons(inPort);
  getAsSockAddrIn()->sin_addr.S_un.S_addr = htonl(inAddress);
}

Socket_Address::Socket_Address(const sockaddr& otherSockaddr)
{
  memcpy(&m_sockddr, &otherSockaddr, sizeof(sockaddr));
}

uint64 Socket_Address::getSize() const
{
  return sizeof(sockaddr);
}

sockaddr_in* Socket_Address::getAsSockAddrIn()
{
  return reinterpret_cast<sockaddr_in*>(&m_sockddr);
}

}