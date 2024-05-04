#include <network/udp_socket.hpp>

// !TODO handle error
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

UDP_Socket_Bind_Result UDP_Socket::bind(const Socket_Address& inBindAddress)
{
  int err = ::bind(m_socket, &inBindAddress.m_sockddr, inBindAddress.getSize());
  if (err == SOCKET_ERROR)
  {
    return UDP_SOCKET_BIND_RESULT_UNKOWN_ERROR;
  }

  return UDP_SOCKET_BIND_RESULT_SUCCESS;
}

UDP_Socket_Send_Result UDP_Socket::send(const span<const char>& inData, const Socket_Address& inToAddress)
{
  int byteSendCount = ::sendto(m_socket, inData.data(), inData.size_bytes(), 0, &inToAddress.m_sockddr, inToAddress.getSize());
  if (byteSendCount == SOCKET_ERROR)
  {
    return UDP_SOCKET_SEND_RESULT_UNKOWN_ERROR;
  }

  return UDP_SOCKET_SEND_RESULT_SUCCESS;
}

UDP_Socket_Recv_Result UDP_Socket::receive(const span<char>& outBuffer, Socket_Address& outFromAddress)
{
  int fromLength = outFromAddress.getSize();
  int readByteCount = ::recvfrom(m_socket, outBuffer.data(), outBuffer.size_bytes(), 0, &outFromAddress.m_sockddr, &fromLength);
  if (readByteCount == SOCKET_ERROR)
  {
    return UDP_SOCKET_RECV_RESULT_UNKOWN_ERROR;
  }

  return UDP_SOCKET_RECV_RESULT_SUCCESS;
}

}