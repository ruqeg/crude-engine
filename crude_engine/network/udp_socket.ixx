module;

#include <winsock2.h>
#include <ws2tcpip.h>

export module crude.network.udp_socket;

import crude.core.std_containers_heap;
import crude.core.std_containers_stack;
import crude.core.alias;
import crude.network.socket_address;

export namespace crude::network
{

class UDP_Socket;
using UDP_Socket_Ptr = core::shared_ptr<UDP_Socket>;

class UDP_Socket
{
public:
  UDP_Socket(SOCKET inSocket);
  UDP_Socket() = delete;
  ~UDP_Socket();

public:
  core::int64 bind(const Socket_Address& inBindAddress);
  core::int64 send(const core::span<const byte>& inData, const Socket_Address& inToAddress);
  core::int64 receive(const core::span<byte>& outBuffer, Socket_Address& outFromAddress);
  core::int64 setNonBlockingMode(bool inShouldBeNonBlocking);

private:
  SOCKET m_socket;

  friend class Socket_Util;
};

}