module;

#include <winsock2.h>
#include <ws2tcpip.h>

export module crude_engine.network.socket_util;
import crude_engine.core.std_containers_heap;
import crude_engine.network.udp_socket;
import crude_engine.network.tcp_socket;

export namespace crude_engine
{

enum Socket_Address_Family
{
  SOCKET_ADDRESS_FAMILT_INET  = AF_INET,
  //SOCKET_ADDRESS_FAMILT_INET6 = AF_INET6
};

class Socket_Util
{
public:
  static UDP_Socket_Ptr createUDPSocket(Socket_Address_Family inFamily);
  static TCP_Socket_Ptr createTCPSocket(Socket_Address_Family inFamily);
  static fd_set* fillSetFromArray(fd_set& outSet, const vector<TCP_Socket_Ptr>* inSockets);
  static void fillArrayFromSet(vector<TCP_Socket_Ptr>*        outSockets,
                               const vector<TCP_Socket_Ptr>*  inSockets,
                               const fd_set&                  inSet);
  static int select(const vector<TCP_Socket_Ptr>*  inReadSet = nullptr,
                    vector<TCP_Socket_Ptr>*        outReadSet = nullptr,
                    const vector<TCP_Socket_Ptr>*  inWriteSet = nullptr,
                    vector<TCP_Socket_Ptr>*        outWriteSet = nullptr,
                    const vector<TCP_Socket_Ptr>*  inExceptSet = nullptr,
                    vector<TCP_Socket_Ptr>*        outExceptSet = nullptr);
};

}