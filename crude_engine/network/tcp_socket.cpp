#include <winsock2.h>
#include <ws2tcpip.h>

module crude_engine.network.tcp_socket;

namespace crude_engine
{

TCP_Socket::~TCP_Socket()
{
  ::closesocket(m_socket);
}

TCP_Socket::TCP_Socket(SOCKET inSocket)
  : 
  m_socket(inSocket)
{}

int64 TCP_Socket::connect(const Socket_Address& inAddress)
{
  int result = ::connect(m_socket, &inAddress.m_sockddr, inAddress.getSize());
  return result;
}

int64 TCP_Socket::bind(const Socket_Address& inToAddress)
{
  int result = ::bind(m_socket, &inToAddress.m_sockddr, inToAddress.getSize());
  return result;
}

int64 TCP_Socket::listen(int64 inBackLog)
{
  int result = ::listen(m_socket, inBackLog);
  return result;
}

TCP_Socket_Ptr TCP_Socket::accept(Socket_Address& inFromAddress)
{
  int length = inFromAddress.getSize();
  SOCKET s = ::accept(m_socket, &inFromAddress.m_sockddr, &length);

  if (s == INVALID_SOCKET)
  {
    return nullptr;
  }

  return makeShared<TCP_Socket>(s);
}

int64 TCP_Socket::send(const span<const char>& inData)
{
  int bytesSendCount = ::send(m_socket, inData.data(), inData.size_bytes(), 0);
  return bytesSendCount;
}

int64 TCP_Socket::receive(const span<char>& outBuffer)
{
  int bytesReceivedCount = ::recv(m_socket, outBuffer.data(), outBuffer.size_bytes(), 0);
  return bytesReceivedCount;
}

int64 TCP_Socket::setNonBlockingMode(bool inShouldBeNonBlocking)
{
  u_long arg = inShouldBeNonBlocking ? 1 : 0;
  int result = ioctlsocket(m_socket, FIONBIO, &arg);
  return result;
}

}