module;

#include <winsock2.h>
#include <ws2tcpip.h>

export module crude_engine.network.udp_socket;

import crude_engine.core.shared_ptr;
import crude_engine.core.std_containers_stack;
import crude_engine.core.alias;
import crude_engine.network.socket_address;

export namespace crude_engine
{

class UDP_Socket;
using UDP_Socket_Ptr = Shared_Ptr<UDP_Socket>;

class UDP_Socket
{
public:
  UDP_Socket(SOCKET inSocket);
  UDP_Socket() = delete;
  ~UDP_Socket();

public:
  int64 bind(const Socket_Address& inBindAddress);
  int64 send(const span<const byte>& inData, const Socket_Address& inToAddress);
  int64 receive(const span<byte>& outBuffer, Socket_Address& outFromAddress);
  int64 setNonBlockingMode(bool inShouldBeNonBlocking);

private:
  SOCKET m_socket;

  friend class Socket_Util;
};

}