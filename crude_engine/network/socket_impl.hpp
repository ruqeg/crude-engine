#pragma once

#include <network/socket_handle.hpp>
#include <network/socket_address.hpp>

namespace crude_engine
{

class Socket_Impl
{
public:
  enum Bind_Result
  {
    BIND_RESULT_SUCCESS,
    BIND_RESULT_UNKNOWN_ERROR,
  };

  enum Send_Result
  {
    SEND_RESULT_SUCCESS,
    SEND_RESULT_UNKNOWN_ERROR,
  };

public:
  static Send_Result send(Socket_Handle inSocket, const void* inData, int64 inLen, const Socket_Address& inToAddress);
  static Bind_Result bind(Socket_Handle inSocket, const Socket_Address& inToAddress);
  static void close(Socket_Handle inSocket);

  friend class Socket_Address;
};

}