module;

#pragma comment(lib, "Ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>

export module crude_engine.network.socket_address;

export import crude_engine.core.alias;

export namespace crude_engine
{

class Socket_Address
{
public:
  Socket_Address() = default;
  Socket_Address(uint32 inAddress, uint16 inPort);
  Socket_Address(const sockaddr& otherSockaddr);

  uint64 getSize() const;

private:
  sockaddr_in* getAsSockAddrIn();

private:
  sockaddr m_sockddr;

  friend class UDP_Socket;
  friend class TCP_Socket;
};

}