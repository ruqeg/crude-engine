#ifdef _CRUDE_EXAMPLE_BASIC_NETWORRK

#include <Windows.h>
#include <iostream>

import crude_engine.core.std_containers_heap;
import crude_engine.network.socket_util;
import crude_engine.network.tcp_socket;
import crude_engine.network.socket_address;

void tcpLoop()
{
  crude_engine::TCP_Socket_Ptr listenSocket = crude_engine::Socket_Util::createTCPSocket(crude_engine::SOCKET_ADDRESS_FAMILT_INET);
  crude_engine::Socket_Address receivingAddres(INADDR_ANY, 4800);
  if (listenSocket->bind(receivingAddres) == SOCKET_ERROR)
  {
    return;
  }

  crude_engine::vector<crude_engine::TCP_Socket_Ptr> readBlockSockets;
  readBlockSockets.push_back(listenSocket);

  crude_engine::vector<crude_engine::TCP_Socket_Ptr> readableSockets;

  constexpr bool isGameRunning = true;
  while (isGameRunning)
  {
    if (crude_engine::Socket_Util::select(&readBlockSockets, &readableSockets))
    {
      for (const crude_engine::TCP_Socket_Ptr& socket : readableSockets)
      {
        if (socket == listenSocket)
        {
          crude_engine::Socket_Address newClientAddress;
          crude_engine::TCP_Socket_Ptr newSocket = listenSocket->accept(newClientAddress);
          readBlockSockets.push_back(newSocket);
        }
        else
        {
          char segment[256];
          int dataReceived = socket->receive(segment);
          if (dataReceived > 0)
          {
            // !TODO
          }

        }
      }
    }
  }
}

int APIENTRY wWinMain(
  _In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPWSTR lpCmdLine,
  _In_ int nCmdShow)
{
  // init console
  AllocConsole();
  FILE* dummy;
  auto s = freopen_s(&dummy, "CONOUT$", "w", stdout);
  tcpLoop();
  return EXIT_SUCCESS;
}
#endif