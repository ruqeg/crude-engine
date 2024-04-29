#pragma once

#include <core/alias.hpp>
#include <core/utility.hpp>

#include <winsock2.h>
#include <ws2tcpip.h>

namespace crude_engine
{

class Socket_Address
{
public:
  Socket_Address(const char* address, uint16 port);
  Socket_Address(const sockaddr& otherSockaddr);

  uint64 getSize() const;

private:
  sockaddr_in6* getAsSockAddrIn6();

private:
  sockaddr m_handle;
};

}