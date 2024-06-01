#include <winsock2.h>
#include <ws2tcpip.h>

module crude.network.socket_util;

import crude.core.assert;
import crude.core.shared_ptr;

namespace crude::network
{

UDP_Socket_Ptr Socket_Util::createUDPSocket(Socket_Address_Family inFamily)
{
  SOCKET s = ::socket(static_cast<core::int32>(inFamily), SOCK_DGRAM, IPPROTO_UDP);
  if (s == INVALID_SOCKET)
  {
    core::assert("TODO" && false);
    return nullptr;
  }
  return core::makeShared<UDP_Socket>(s);
}

TCP_Socket_Ptr Socket_Util::createTCPSocket(Socket_Address_Family inFamily)
{
  SOCKET s = ::socket(static_cast<core::int32>(inFamily), SOCK_STREAM, IPPROTO_TCP);
  if (s == INVALID_SOCKET)
  {
    core::assert("TODO" && false);
    return nullptr;
  }
  return core::makeShared<TCP_Socket>(s);
}

fd_set* Socket_Util::fillSetFromArray(fd_set& outSet, const core::vector<TCP_Socket_Ptr>* inSockets)
{
  if (inSockets == nullptr)
  {
    return nullptr;
  }

  FD_ZERO(&outSet);
  for (const TCP_Socket_Ptr& socket : *inSockets)
  {
    FD_SET(socket->m_socket, &outSet);
  }
  return &outSet;
}

void Socket_Util::fillArrayFromSet(core::vector<TCP_Socket_Ptr>*        outSockets,
                                   const core::vector<TCP_Socket_Ptr>*  inSockets,
                                   const fd_set&                        inSet)
{
  if ((inSockets == nullptr) || (outSockets == nullptr))
  {
    return;
  }

  outSockets->clear();
  for (const TCP_Socket_Ptr& socket : *inSockets)
  {
    if (FD_ISSET(socket->m_socket, &inSet))
    {
      outSockets->push_back(socket);
    }
  }
};

int Socket_Util::select(const core::vector<TCP_Socket_Ptr>*  inReadSet,
                        core::vector<TCP_Socket_Ptr>*        outReadSet,
                        const core::vector<TCP_Socket_Ptr>*  inWriteSet,
                        core::vector<TCP_Socket_Ptr>*        outWriteSet,
                        const core::vector<TCP_Socket_Ptr>*  inExceptSet,
                        core::vector<TCP_Socket_Ptr>*        outExceptSet)
{
  fd_set read, write, except;

  fd_set* readPtr = fillSetFromArray(read, inReadSet);
  fd_set* writePtr = fillSetFromArray(read, inWriteSet);
  fd_set* exceptPtr = fillSetFromArray(read, inExceptSet);

  core::int64 toRet = ::select(0, readPtr, writePtr, exceptPtr, nullptr);

  if (toRet > 0)
  {
    fillArrayFromSet(outReadSet, inReadSet, read);
    fillArrayFromSet(outWriteSet, inWriteSet, write);
    fillArrayFromSet(outExceptSet, inExceptSet, except);
  }

  return toRet;
}

}