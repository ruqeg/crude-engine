#pragma once

#include <core/std_containers_naloc.hpp>
#include <network/socket_address.hpp>

namespace crude_engine
{

enum UDP_Socket_Bind_Result
{
  UDP_SOCKET_BIND_RESULT_SUCCESS,
  UDP_SOCKET_BIND_RESULT_UNKOWN_ERROR,
};

enum UDP_Socket_Send_Result
{
  UDP_SOCKET_SEND_RESULT_SUCCESS,
  UDP_SOCKET_SEND_RESULT_UNKOWN_ERROR,
};

enum UDP_Socket_Recv_Result
{
  UDP_SOCKET_RECV_RESULT_SUCCESS,
  UDP_SOCKET_RECV_RESULT_UNKOWN_ERROR,
};

class UDP_Socket
{
public:
  UDP_Socket() = delete;
  UDP_Socket(SOCKET inSocket);
  ~UDP_Socket();

public:
  UDP_Socket_Bind_Result bind(const Socket_Address& inBindAddress);
  UDP_Socket_Send_Result send(const span<const char>& inData, const Socket_Address& inToAddress);
  UDP_Socket_Recv_Result receive(const span<char>& outBuffer, Socket_Address& outFromAddress);

private:
  SOCKET m_socket;
};

}