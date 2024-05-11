module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.instance;

import crude_engine.core.shared_ptr;
import crude_engine.core.std_containers_stack;
import crude_engine.core.std_containers_heap;
import crude_engine.graphics.vulkan.vulkan_object;
import crude_engine.graphics.vulkan.application;

export namespace crude_engine
{

class Physical_Device;

class Instance : public Vulkan_Object<VkInstance>
{
public:
  explicit Instance(
#ifdef VK_EXT_debug_utils
                    PFN_vkDebugUtilsMessengerCallbackEXT   debugUtilsCallback,
#endif // VK_EXT_debug_utils
                     const Application&                    application        = Application(),
                     const span<const char*>&              enabledExtensions  = {},
                     const span<const char*>&              enabledLayers      = {},
                     VkInstanceCreateFlags                 flags              = 0u);
  ~Instance();
  vector<Shared_Ptr<Physical_Device>> getPhysicalDevices();
};

}
