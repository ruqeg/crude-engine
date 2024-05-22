#include <winsock2.h>
#include <ws2tcpip.h>

module crude.network.socket_address_factory;

import crude.core.assert;
import crude.core.std_containers_stack;

namespace crude::network
{

Socket_Address_Ptr Socket_Address_Factory::createIPv4FromString(const char* inString)
{
  auto pos = strchr(inString, ':');

  core::array<char, 256> host{};
  core::array<char, 256> service{};
  if (pos != nullptr)
  {
    memcpy(host.data(), inString, pos - inString);
    strcpy(service.data(), pos + 1);
  }
  else
  {
    strcpy(host.data(), inString);
  }

  addrinfo hint{};
  hint.ai_family = AF_INET;

  addrinfo* result;
  int error = getaddrinfo(host.data(), service.data(), &hint, &result);

  if (error != 0 && result != nullptr)
  {
    freeaddrinfo(result);
    return nullptr;
  }

  while (!result->ai_addr && result->ai_next)
  {
    result = result->ai_next;
  }
  
  if (!result->ai_addr)
  {
    freeaddrinfo(result);
    return nullptr;
  }

  auto toRet = core::makeShared<Socket_Address>(*result->ai_addr);
  freeaddrinfo(result);
  return toRet;
}

}