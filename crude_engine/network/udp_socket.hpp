#pragma once

#include <core/std_containers_naloc.hpp>
#include <core/shared_ptr.hpp>
#include <network/socket_address.hpp>

namespace crude_engine
{

class UDP_Socket;
using UDP_Socket_Ptr = Shared_Ptr<UDP_Socket>;

class UDP_Socket
{
public:
  UDP_Socket() = delete;
  UDP_Socket(SOCKET inSocket);
  ~UDP_Socket();

public:
  int64 bind(const Socket_Address& inBindAddress);
  int64 send(const span<const char>& inData, const Socket_Address& inToAddress);
  int64 receive(const span<char>& outBuffer, Socket_Address& outFromAddress);
  int64 setNonBlockingMode(bool inShouldBeNonBlocking);

private:
  SOCKET m_socket;

  friend class Socket_Util;
};

}