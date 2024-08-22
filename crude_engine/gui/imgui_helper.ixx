module;

export module crude.gui.imgui_helper;

export import crude.core.std_containers_heap;

export namespace crude::platform
{

class SDL_Window_Container;

}

export namespace crude::gui
{

void initializeImGuiContext();
void initializeImGuiVulkanSDL(core::shared_ptr<platform::SDL_Window_Container> windowContainer);
void deinitializeImGuiVulkanSDL();
void deinitializeImGuiContext();

}
