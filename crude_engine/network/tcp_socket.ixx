#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

export module crude.network.tcp_socket;

import crude.core.alias;
import crude.core.std_containers_stack;
import crude.core.std_containers_heap;
import crude.network.socket_address;

export namespace crude::network
{

class TCP_Socket;
using TCP_Socket_Ptr = core::shared_ptr<TCP_Socket>;

class TCP_Socket
{
public:
  TCP_Socket(SOCKET inSocket);
  TCP_Socket() = delete;
  ~TCP_Socket();

public:
  core::int64 connect(const Socket_Address& inAddress);
  core::int64 bind(const Socket_Address& inToAddress);
  core::int64 listen(core::int64 inBackLog = SOMAXCONN);
  TCP_Socket_Ptr accept(Socket_Address& inFromAddress);
  core::int64 send(const core::span<const byte>& inData);
  core::int64 receive(const core::span<byte>& outBuffer);
  core::int64 setNonBlockingMode(bool inShouldBeNonBlocking);

private:
  SOCKET m_socket;

  friend class Socket_Util;
};

}