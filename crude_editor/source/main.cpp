#include <flecs.h>
#define VK_USE_PLATFORM_WIN32_KHR
#include <Windows.h>

import crude.editor.application;

int APIENTRY wWinMain(_In_ HINSTANCE      hInstance,
                      _In_opt_ HINSTANCE  hPrevInstance,
                      _In_ LPWSTR         lpCmdLine,
                      _In_ int            nCmdShow)
{
  crude::Engine::preinitialize(600000000);
  {
    crude::editor::Application application;
    application.initialize();
    application.run();
    application.deinitialize();
  }
  crude::Engine::postdeinitialize();
  return EXIT_SUCCESS;
}
