#pragma once

#include <core/std_containers_naloc.hpp>
#include <core/shared_ptr.hpp>
#include <network/socket_address.hpp>

namespace crude_engine
{

enum TCP_Socket_Connect_Result
{
  TCP_SOCKET_CONNECT_RESULT_SUCCESS,
  TCP_SOCKET_CONNECT_RESULT_UNKOWN_ERROR,
};

enum TCP_Socket_Bind_Result
{
  TCP_SOCKET_BIND_RESULT_SUCCESS,
  TCP_SOCKET_BIND_RESULT_UNKOWN_ERROR,
};

enum TCP_Socket_Listen_Result
{
  TCP_SOCKET_LISTEN_RESULT_SUCCESS,
  TCP_SOCKET_LISTEN_RESULT_UNKOWN_ERROR,
};

enum TCP_Socket_Accept_Result
{
  TCP_SOCKET_ACCEPT_RESULT_SUCCESS,
  TCP_SOCKET_ACCEPT_RESULT_UNKOWN_ERROR,
};

enum TCP_Socket_Send_Result
{
  TCP_SOCKET_SEND_RESULT_SUCCESS,
  TCP_SOCKET_SEND_RESULT_UNKOWN_ERROR,
};

enum TCP_Socket_Recv_Result
{
  TCP_SOCKET_RECV_RESULT_SUCCESS,
  TCP_SOCKET_RECV_RESULT_UNKOWN_ERROR,
};

class TCP_Socket
{
public:
  TCP_Socket() = delete;
  ~TCP_Socket();
  TCP_Socket(SOCKET inSocket);

public:
  TCP_Socket_Connect_Result connect(const Socket_Address& inAddress);
  TCP_Socket_Bind_Result bind(const Socket_Address& inToAddress);
  TCP_Socket_Listen_Result listen(int64 inBackLog = SOMAXCONN);
  Shared_Ptr<TCP_Socket> accept(Socket_Address& inFromAddress);
  TCP_Socket_Send_Result send(const span<const char>& inData);
  TCP_Socket_Recv_Result receive(const span<char>& outBuffer);

private:
  SOCKET m_socket;
};

}