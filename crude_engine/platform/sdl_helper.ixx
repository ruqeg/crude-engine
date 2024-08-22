module;

export module crude.platform.sdl_helper;

export namespace crude::platform
{
 
bool initializeSDL();
bool loadSDLVulkan();
void unloadSDLVulkan();
void deinitializeSDL();
bool isSDLInitialized();

}
