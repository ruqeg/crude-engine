#ifdef _WIN32

#include <network/socket_impl.hpp>

#include <winsock2.h>
#include <ws2tcpip.h>

namespace crude_engine
{

//Socket_Impl::Send_Result Socket_Impl::send(Socket_Handle inSocket, const void* inData, int64 inLen, const Socket_Address& inToAddress)
//{
//  int result = ::sendto(inSocket, static_cast<const char*>(inData), inLen, 0u, &inToAddress.m_handle, inToAddress.getSize());
//  
//  if (result == SOCKET_ERROR)
//    return SEND_RESULT_UNKNOWN_ERROR;
//
//  return SEND_RESULT_SUCCESS;
//}
//
//Socket_Impl::Bind_Result Socket_Impl::bind(Socket_Handle inSocket, const Socket_Address& inToAddress)
//{
//  int result = ::bind(inSocket, &inToAddress.m_handle, inToAddress.getSize());
//
//  if (result == SOCKET_ERROR)
//    return BIND_RESULT_UNKNOWN_ERROR;
//
//  return BIND_RESULT_SUCCESS;
//}

void Socket_Impl::close(Socket_Handle inSocket)
{
  closesocket(inSocket);
}

struct Socket_Initializer
{
  Socket_Initializer()
  {
    WSADATA init;
    WSAStartup(MAKEWORD(2, 2), &init);
  }

  ~Socket_Initializer()
  {
    WSACleanup();
  }
};

Socket_Initializer gSocketInitializer;

}

#endif