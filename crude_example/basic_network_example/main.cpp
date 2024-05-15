#ifdef _CRUDE_EXAMPLE_BASIC_NETWORRK

#include <Windows.h>
#include <iostream>
#include <thread>

import crude_engine.core.std_containers_heap;
import crude_engine.network.socket_util;
import crude_engine.network.tcp_socket;
import crude_engine.network.socket_address;
import crude_engine.network.socket_address_factory;

void tcpServerLoop()
{
  crude_engine::TCP_Socket_Ptr listenSocket = crude_engine::Socket_Util::createTCPSocket(crude_engine::SOCKET_ADDRESS_FAMILT_INET);
  crude_engine::Socket_Address receivingAddres(crude_engine::Socket_Address_Init_IPv4(INADDR_ANY, 54000));

  if (listenSocket->bind(receivingAddres) == SOCKET_ERROR)
  {
    return;
  }

  if (listenSocket->listen() == SOCKET_ERROR)
  {
    return;
  }

  crude_engine::vector<crude_engine::TCP_Socket_Ptr> readBlockSockets;
  readBlockSockets.push_back(listenSocket);

  crude_engine::vector<crude_engine::TCP_Socket_Ptr> readableSockets;
  crude_engine::vector<crude_engine::TCP_Socket_Ptr> writeSockets;
  
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
          writeSockets.push_back(newSocket);
        }
        else
        {
          char segment[256]{};
          int dataReceived = socket->receive(segment);
          if (dataReceived > 0)
          {
            std::cout << "Message from client: " << segment << std::endl;
          }

          for (const crude_engine::TCP_Socket_Ptr& sendSocket : writeSockets)
          {
            if (sendSocket != socket)
            {
              if (sendSocket->send(segment) == SOCKET_ERROR)
              {
                std::cout << "Failed send message to client: " << segment << std::endl;
              }
            }
          }
        }
      }
    }
  }
}

void tcpClientLoop()
{
  crude_engine::TCP_Socket_Ptr sendSocket = crude_engine::Socket_Util::createTCPSocket(crude_engine::SOCKET_ADDRESS_FAMILT_INET);
  crude_engine::Socket_Address sendingAddres(crude_engine::Socket_Address_Init_IPv4("127.0.0.1", 54000));
  
  if (sendSocket->connect(sendingAddres) == SOCKET_ERROR)
  {
    return;
  }

  while (true)
  {
    std::thread readerThread([&sendSocket]() -> void {
      while (true)
      {
        std::cout << "Enter message: ";
        char sendMsg[256]{};
        fgets(sendMsg, sizeof(sendMsg), stdin);
        std::cout << "\n";
        if (sendSocket->send(sendMsg) == SOCKET_ERROR)
        {
          return;
        }
      }
    });

    std::thread writerThread([&sendSocket]() -> void {
      while (true)
      {
        char recvMsg[256]{};
        if (sendSocket->receive(recvMsg) > 0)
        {
          std::cout << "Recieve message: " << recvMsg << "\n" << std::endl;
        }
      }
    });
    
    readerThread.join();
    writerThread.join();
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
  freopen_s(&dummy, "CONOUT$", "w", stdout);
  freopen_s(&dummy, "CONIN$", "r", stdin);
  
  int apptype;
  std::cin >> apptype;
  if (apptype == 0)
    tcpServerLoop();
  else if (apptype == 1)
    tcpClientLoop();
  
  return EXIT_SUCCESS;
}
#endif