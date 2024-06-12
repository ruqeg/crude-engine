#include <winsock2.h>
#include <ws2tcpip.h>

module crude.network.tcp_socket;

namespace crude::network
{

TCP_Socket::~TCP_Socket()
{
  ::closesocket(m_socket);
}

TCP_Socket::TCP_Socket(SOCKET inSocket)
  : 
  m_socket(inSocket)
{}

core::int64 TCP_Socket::connect(const Socket_Address& inAddress)
{
  int result = ::connect(m_socket, &inAddress.m_sockddr, inAddress.getSize());
  return result;
}

core::int64 TCP_Socket::bind(const Socket_Address& inToAddress)
{
  int result = ::bind(m_socket, &inToAddress.m_sockddr, inToAddress.getSize());
  return result;
}

core::int64 TCP_Socket::listen(core::int64 inBackLog)
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

  return core::allocateShared<TCP_Socket>(s);
}

core::int64 TCP_Socket::send(const core::span<const byte>& inData)
{
  static_assert(sizeof(byte) == sizeof(char));
  int bytesSendCount = ::send(m_socket, reinterpret_cast<const char*>(inData.data()), inData.size_bytes(), 0);
  return bytesSendCount;
}

core::int64 TCP_Socket::receive(const core::span<byte>& outBuffer)
{
  static_assert(sizeof(byte) == sizeof(char));
  int bytesReceivedCount = ::recv(m_socket, reinterpret_cast<char*>(outBuffer.data()), outBuffer.size_bytes(), 0);
  return bytesReceivedCount;
}

core::int64 TCP_Socket::setNonBlockingMode(bool inShouldBeNonBlocking)
{
  u_long arg = inShouldBeNonBlocking ? 1 : 0;
  int result = ioctlsocket(m_socket, FIONBIO, &arg);
  return result;
}

}