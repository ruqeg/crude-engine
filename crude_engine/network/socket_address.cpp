#include <winsock2.h>
#include <ws2tcpip.h>

module crude_engine.network.socket_address;

import crude_engine.core.string;

namespace crude_engine
{

Socket_Address::Socket_Address()
  :
  m_family(SOCKET_ADDRESS_FAMILT_UNSPEC)
{
}

Socket_Address::Socket_Address(const Socket_Address_Init_IPv4& inData)
  :
  m_family(SOCKET_ADDRESS_FAMILT_INET)
{
  getAsSockAddrIn()->sin_family = AF_INET;
  getAsSockAddrIn()->sin_port   = inData.inPort;
  getAsSockAddrIn()->sin_addr   = inData.inAddress;
}

Socket_Address::Socket_Address(const Socket_Address_Init_IPv6& inData)
  :
  m_family(SOCKET_ADDRESS_FAMILT_INET6)
{
  getAsSockAddrIn6()->sin6_family = AF_INET6;
  getAsSockAddrIn6()->sin6_port   = inData.inPort;
  getAsSockAddrIn6()->sin6_addr   = inData.inAddress;
}

Socket_Address::Socket_Address(const sockaddr& otherSockaddr)
  :
  m_family((otherSockaddr.sa_family == AF_INET) ? SOCKET_ADDRESS_FAMILT_INET : SOCKET_ADDRESS_FAMILT_INET6)
{
  memcpy(&m_sockddr, &otherSockaddr, sizeof(sockaddr));
}

uint64 Socket_Address::getSize() const
{
  return sizeof(sockaddr);
}

Socket_Address_Family Socket_Address::getType() const
{
  return m_family;
}

sockaddr_in* Socket_Address::getAsSockAddrIn()
{
  return reinterpret_cast<sockaddr_in*>(&m_sockddr);
}

sockaddr_in6* Socket_Address::getAsSockAddrIn6()
{
  return reinterpret_cast<sockaddr_in6*>(&m_sockddr);
}

Socket_Address_Init_IPv4::Socket_Address_Init_IPv4(const char* inAddress, uint16 inPort)
{
  inet_pton(AF_INET, inAddress, &this->inAddress);
  this->inPort = htons(inPort);
}

Socket_Address_Init_IPv4::Socket_Address_Init_IPv4(uint64 inAddress, uint16 inPort)
{
  this->inAddress.S_un.S_addr = htonl(inAddress);
  this->inPort                = htons(inPort);
}

Socket_Address_Init_IPv6::Socket_Address_Init_IPv6(const char* inAddress, uint16 inPort)
{
  inet_pton(AF_INET6, inAddress, &this->inAddress);
  this->inPort = htons(inPort);
}

}