#pragma once

#include <core/std_containers_naloc.hpp>
#include <core/shared_ptr.hpp>
#include <network/socket_address.hpp>

namespace crude_engine
{

class TCP_Socket;
using TCP_Socket_Ptr = Shared_Ptr<TCP_Socket>;

class TCP_Socket
{
public:
  TCP_Socket() = delete;
  ~TCP_Socket();
  TCP_Socket(SOCKET inSocket);

public:
  int64 connect(const Socket_Address& inAddress);
  int64 bind(const Socket_Address& inToAddress);
  int64 listen(int64 inBackLog = SOMAXCONN);
  TCP_Socket_Ptr accept(Socket_Address& inFromAddress);
  int64 send(const span<const char>& inData);
  int64 receive(const span<char>& outBuffer);

private:
  SOCKET m_socket;

  friend class Socket_Util;
};

}