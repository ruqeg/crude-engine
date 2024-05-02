#pragma once

#include <core/std_containers_naloc.hpp>
#include <network/socket_address.hpp>

namespace crude_engine
{

enum Socket_Bind_Result
{
  SOCKET_BIND_RESULT_SUCCESS,
  SOCKET_BIND_RESULT_UNKOWN_ERROR,
};

enum Socket_Send_Result
{
  SOCKET_SEND_RESULT_SUCCESS,
  SOCKET_SEND_RESULT_UNKOWN_ERROR,
};

enum Socket_Recv_Result
{
  SOCKET_RECV_RESULT_SUCCESS,
  SOCKET_RECV_RESULT_UNKOWN_ERROR,
};

class UDP_Socket
{
public:
  UDP_Socket() = delete;
  UDP_Socket(SOCKET inSocket);
  ~UDP_Socket();

public:
  Socket_Bind_Result bind(const Socket_Address& inBindAddress);
  Socket_Send_Result send(const span<const char>& inData, const Socket_Address& inToAddress);
  Socket_Recv_Result receive(span<char>& outBuffer, Socket_Address& outFromAddress);

private:
  SOCKET m_socket;
};

}