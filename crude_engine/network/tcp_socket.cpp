#include <network/tcp_socket.hpp>

namespace crude_engine
{

// !TODO handle error
TCP_Socket::~TCP_Socket()
{
  closesocket(m_socket);
}

TCP_Socket::TCP_Socket(SOCKET inSocket)
  : 
  m_socket(inSocket)
{}

TCP_Socket_Connect_Result TCP_Socket::connect(const Socket_Address& inAddress)
{
  int err = ::connect(m_socket, &inAddress.m_sockddr, inAddress.getSize());
  if (err == SOCKET_ERROR)
  {
    return TCP_SOCKET_CONNECT_RESULT_UNKOWN_ERROR;
  }
  return TCP_SOCKET_CONNECT_RESULT_SUCCESS;
}

TCP_Socket_Bind_Result TCP_Socket::bind(const Socket_Address& inToAddress)
{
  int err = ::bind(m_socket, &inToAddress.m_sockddr, inToAddress.getSize());
  if (err == SOCKET_ERROR)
  {
    return TCP_SOCKET_BIND_RESULT_UNKOWN_ERROR;
  }

  return TCP_SOCKET_BIND_RESULT_SUCCESS;
}

TCP_Socket_Listen_Result TCP_Socket::listen(int64 inBackLog)
{
  int err = ::listen(m_socket, inBackLog);
  if (err == SOCKET_ERROR)
  {
    return TCP_SOCKET_LISTEN_RESULT_UNKOWN_ERROR;
  }
  return TCP_SOCKET_LISTEN_RESULT_UNKOWN_ERROR;
}

Shared_Ptr<TCP_Socket> TCP_Socket::accept(Socket_Address& inFromAddress)
{
  int length = inFromAddress.getSize();
  SOCKET s = ::accept(m_socket, &inFromAddress.m_sockddr, &length);

  if (s == INVALID_SOCKET)
  {
    return nullptr;
  }

  return makeShared<TCP_Socket>(s);
}

TCP_Socket_Send_Result TCP_Socket::send(const span<const char>& inData)
{
  int bytesSendCount = ::send(m_socket, inData.data(), inData.size_bytes(), 0);
  if (bytesSendCount == SOCKET_ERROR)
  {
    return TCP_SOCKET_SEND_RESULT_UNKOWN_ERROR;
  }
  return TCP_SOCKET_SEND_RESULT_SUCCESS;
}

TCP_Socket_Recv_Result TCP_Socket::receive(const span<char>& outBuffer)
{
  int bytesReceivedCount = ::recv(m_socket, outBuffer.data(), outBuffer.size_bytes(), 0);
  if (bytesReceivedCount == SOCKET_ERROR)
  {
    return TCP_SOCKET_RECV_RESULT_UNKOWN_ERROR;
  }
  return TCP_SOCKET_RECV_RESULT_SUCCESS;
}

}