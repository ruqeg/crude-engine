#ifdef _CRUDE_EXAMPLE_BASIC_NETWORRK

#include <Windows.h>
#include <iostream>
#include <thread>

import crude_engine.core.input_memory_stream;
import crude_engine.core.output_memory_steam;
import crude_engine.core.std_containers_heap;
import crude_engine.network.socket_util;
import crude_engine.network.tcp_socket;
import crude_engine.network.socket_address;
import crude_engine.network.socket_address_factory;

class Game_Object {};

class Robo_Cat : public Game_Object
{
public:
  void write(crude_engine::Output_Memory_System& inStream) const;
  void read(crude_engine::Input_Memory_System& inStream);
public:
  int m_health;
  int m_meowCount;
  Game_Object* m_homeBase;
  char m_name[20];
  crude_engine::vector<int> m_miceIndices;
};

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
          byte segment[256]{};
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
        Robo_Cat r;
        std::cout << "Enter robot health: ";
        std::cin >> r.m_health;
        std::cout << std::endl;
        std::cout << "Enter robot meow: ";
        std::cin >> r.m_meowCount;
        std::cout << std::endl;
        std::cout << "Enter robot name: ";
        std::cin >> r.m_name;
        std::cout << std::endl;

        crude_engine::Output_Memory_System stream;
        r.write(stream);
        if (sendSocket->send(*stream.getBufferPtr()) == SOCKET_ERROR)
        {
          return;
        }
      }
    });

    std::thread writerThread([&sendSocket]() -> void {
      while (true)
      {
        auto buffer = crude_engine::makeShared<crude_engine::vector<byte>>(256);
        if (sendSocket->receive(*buffer) > 0)
        {
          Robo_Cat r;
          crude_engine::Input_Memory_System stream(buffer);
          r.read(stream);
          std::cout
            << std::endl
            << "Recieve robot health: " << r.m_health << std::endl
            << "Recieve robot meow: " << r.m_health << std::endl
            << "Recieve robot name: " << r.m_name << std::endl;
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

void Robo_Cat::write(crude_engine::Output_Memory_System& inStream) const
{
  inStream.write(m_health);
  inStream.write(m_meowCount);
  inStream.write((crude_engine::byte*)m_name, sizeof(m_name));
}

void Robo_Cat::read(crude_engine::Input_Memory_System& inStream) 
{
  inStream.read(m_health);
  inStream.read(m_meowCount);
  inStream.read((crude_engine::byte*)m_name, sizeof(m_name));
}

#endif