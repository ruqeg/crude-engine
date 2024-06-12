module;

#pragma comment(lib, "Ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>

export module crude.network.socket_address;

import crude.core.alias;
import crude.core.std_containers_stack;
import crude.core.std_containers_heap;

export namespace crude::network
{

class Socket_Address_Init_IPv4
{
public:
  Socket_Address_Init_IPv4(const char* inAddress, core::uint16 inPort);
  Socket_Address_Init_IPv4(core::uint64 inAddress, core::uint16 inPort);

private:
  IN_ADDR  inAddress;
  USHORT   inPort;

  friend class Socket_Address;
};

class Socket_Address_Init_IPv6
{
public:
  Socket_Address_Init_IPv6(const char* inAddress, core::uint16 inPort);

private:
  IN6_ADDR  inAddress;
  USHORT    inPort;

  friend class Socket_Address;
};

enum Socket_Address_Family
{
  SOCKET_ADDRESS_FAMILT_UNSPEC = AF_UNSPEC,
  SOCKET_ADDRESS_FAMILT_INET   = AF_INET,
  SOCKET_ADDRESS_FAMILT_INET6  = AF_INET6
};

class Socket_Address;
using Socket_Address_Ptr = core::shared_ptr<Socket_Address>;

class Socket_Address
{
public:
  Socket_Address();
  explicit Socket_Address(const Socket_Address_Init_IPv4& inData);
  explicit Socket_Address(const Socket_Address_Init_IPv6& inData);
  Socket_Address(const sockaddr& otherSockaddr);

  core::uint64 getSize() const;
  Socket_Address_Family getType() const;

private:
  sockaddr_in* getAsSockAddrIn();
  sockaddr_in6* getAsSockAddrIn6();

private:
  sockaddr                     m_sockddr;
  const Socket_Address_Family  m_family;

  friend class UDP_Socket;
  friend class TCP_Socket;
};

}