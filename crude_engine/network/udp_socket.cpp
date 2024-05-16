#include <winsock2.h>
#include <ws2tcpip.h>

module crude_engine.network.udp_socket;

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

int64 UDP_Socket::bind(const Socket_Address& inBindAddress)
{
  int result = ::bind(m_socket, &inBindAddress.m_sockddr, inBindAddress.getSize());
  return result;
}

int64 UDP_Socket::send(const span<const byte>& inData, const Socket_Address& inToAddress)
{
  static_assert(sizeof(byte) == sizeof(char));
  int byteSendCount = ::sendto(m_socket, reinterpret_cast<const char*>(inData.data()), inData.size_bytes(), 0, &inToAddress.m_sockddr, inToAddress.getSize());
  return byteSendCount;
}

int64 UDP_Socket::receive(const span<byte>& outBuffer, Socket_Address& outFromAddress)
{
  static_assert(sizeof(byte) == sizeof(char));
  int fromLength = outFromAddress.getSize();
  int readByteCount = ::recvfrom(m_socket, reinterpret_cast<char*>(outBuffer.data()), outBuffer.size_bytes(), 0, &outFromAddress.m_sockddr, &fromLength);
  return readByteCount;
}

int64 UDP_Socket::setNonBlockingMode(bool inShouldBeNonBlocking)
{
  u_long arg = inShouldBeNonBlocking ? 1 : 0;
  int result = ioctlsocket(m_socket, FIONBIO, &arg);
  return result;
}

}