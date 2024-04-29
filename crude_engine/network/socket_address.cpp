#include <network/socket_address.hpp>

namespace crude_engine
{

Socket_Address::Socket_Address(const char* address, uint16 port)
{
  *getAsSockAddrIn6() = {};
  getAsSockAddrIn6()->sin6_family = AF_INET6;
  getAsSockAddrIn6()->sin6_port = htons(port);
  inet_pton(AF_INET6, address, &getAsSockAddrIn6()->sin6_addr);
}

Socket_Address::Socket_Address(const sockaddr& otherSockaddr)
{
  memcpy(&m_handle, &otherSockaddr, sizeof(sockaddr));
}

uint64 Socket_Address::getSize() const
{
  return sizeof(sockaddr);
}

sockaddr_in6* Socket_Address::getAsSockAddrIn6()
{
  return reinterpret_cast<sockaddr_in6*>(&m_handle);
}

}