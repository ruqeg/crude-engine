#define VK_USE_PLATFORM_WIN32_KHR
#include <Windows.h>

import application;

int APIENTRY wWinMain(_In_ HINSTANCE      hInstance,
                      _In_opt_ HINSTANCE  hPrevInstance,
                      _In_ LPWSTR         lpCmdLine,
                      _In_ int            nCmdShow)
{
  crude::Engine::initialize({1000000});
  auto windowContainer = crude::core::makeShared<crude::system::SDL_Window_Container>(
    "TEST", 600, 600, crude::system::SDL_WINDOW_CONTAINER_FLAG_VULKAN);
  Application application(windowContainer);
  application.run();
  return EXIT_SUCCESS;
}
