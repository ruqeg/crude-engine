module;

#include <winsock2.h>
#include <ws2tcpip.h>

export module crude.network.socket_util;

import crude.core.std_containers_heap;
import crude.network.udp_socket;
import crude.network.tcp_socket;
import crude.network.socket_address;

export namespace crude::network
{

class Socket_Util
{
public:
  static UDP_Socket_Ptr createUDPSocket(Socket_Address_Family inFamily);
  static TCP_Socket_Ptr createTCPSocket(Socket_Address_Family inFamily);
  static fd_set* fillSetFromArray(fd_set& outSet, const core::vector<TCP_Socket_Ptr>* inSockets);
  static void fillArrayFromSet(core::vector<TCP_Socket_Ptr>*        outSockets,
                               const core::vector<TCP_Socket_Ptr>*  inSockets,
                               const fd_set&                        inSet);
  static int select(const core::vector<TCP_Socket_Ptr>*  inReadSet = nullptr,
                    core::vector<TCP_Socket_Ptr>*        outReadSet = nullptr,
                    const core::vector<TCP_Socket_Ptr>*  inWriteSet = nullptr,
                    core::vector<TCP_Socket_Ptr>*        outWriteSet = nullptr,
                    const core::vector<TCP_Socket_Ptr>*  inExceptSet = nullptr,
                    core::vector<TCP_Socket_Ptr>*        outExceptSet = nullptr);
};

}