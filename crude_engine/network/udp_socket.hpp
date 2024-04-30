#pragma once

#include <network/socket_impl.hpp>
#include <network/socket_address.hpp>

namespace crude_engine
{

class UDP_Socket
{
public:
  UDP_Socket() = delete;
  ~UDP_Socket();

public:
  int bind(const Socket_Address& inToAddress);
  int send(const void* inData, int64 inLen, const Socket_Address& inToAddress);
  int receive(const void* inBuffer, int64 inLen, Socket_Address& outFromAddress);

private:
  Socket_Handle m_socket;
};

}