#define VK_USE_PLATFORM_WIN32_KHR
#include <Windows.h>

import application;

int APIENTRY wWinMain(_In_ HINSTANCE      hInstance,
                      _In_opt_ HINSTANCE  hPrevInstance,
                      _In_ LPWSTR         lpCmdLine,
                      _In_ int            nCmdShow)
{
  crude::Engine::initialize({1000000});
  Application application;
  application.run();
  return EXIT_SUCCESS;
}
