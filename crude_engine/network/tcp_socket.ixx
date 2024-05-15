#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

export module crude_engine.network.tcp_socket;

import crude_engine.core.shared_ptr;
import crude_engine.core.alias;
import crude_engine.core.std_containers_stack;
import crude_engine.network.socket_address;

export namespace crude_engine
{

class TCP_Socket;
using TCP_Socket_Ptr = Shared_Ptr<TCP_Socket>;

class TCP_Socket
{
public:
  TCP_Socket(SOCKET inSocket);
  TCP_Socket() = delete;
  ~TCP_Socket();

public:
  int64 connect(const Socket_Address& inAddress);
  int64 bind(const Socket_Address& inToAddress);
  int64 listen(int64 inBackLog = SOMAXCONN);
  TCP_Socket_Ptr accept(Socket_Address& inFromAddress);
  int64 send(const span<const char>& inData);
  int64 receive(const span<char>& outBuffer);

public:
  SOCKET m_socket;

  friend class Socket_Util;
};

}