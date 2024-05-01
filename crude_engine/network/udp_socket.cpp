#include <network/udp_socket.hpp>

namespace crude_engine
{

UDP_Socket::UDP_Socket(SOCKET inSocket)
  :
  m_socket(inSocket)
{}

UDP_Socket::~UDP_Socket()
{
  closesocket(m_socket);
}

Socket_Bind_Result UDP_Socket::bind(const Socket_Address& inBindAddress)
{
  int res = ::bind(m_socket, &inBindAddress.m_sockddr, inBindAddress.getSize());
  if (res == SOCKET_ERROR)
  {
    // !TODO handle error
    return SOCKET_BIND_RESULT_UNKOWN_ERROR;
  }

  return SOCKET_BIND_RESULT_SUCCESS;
}

Socket_Send_Result UDP_Socket::send(const void* inData, int64 inLen, const Socket_Address& inToAddress)
{
  int byteSendCount = ::sendto(m_socket, static_cast<const char*>(inData), inLen, 0, &inToAddress.m_sockddr, inToAddress.getSize());
  if (byteSendCount == SOCKET_ERROR)
  {
    // !TODO handle error
    return SOCKET_SEND_RESULT_UNKOWN_ERROR;
  }

  return SOCKET_SEND_RESULT_SUCCESS;
}

Socket_Recv_Result UDP_Socket::receive(void* outBuffer, int64 inMaxLen, Socket_Address& outFromAddress)
{
  int fromLength = outFromAddress.getSize();
  int readByteCount = ::recvfrom(m_socket, static_cast<char*>(outBuffer), inMaxLen, 0, &outFromAddress.m_sockddr, &fromLength);
  if (readByteCount == SOCKET_ERROR)
  {
    // !TODO handle error
    return SOCKET_RECV_RESULT_UNKOWN_ERROR;
  }

  return SOCKET_RECV_RESULT_SUCCESS;
}

}