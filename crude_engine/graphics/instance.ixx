module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.vulkan.instance;

import crude.core.shared_ptr;
import crude.core.std_containers_stack;
import crude.core.std_containers_heap;
import crude.graphics.vulkan.vulkan_object;
import crude.graphics.vulkan.application;

export namespace crude::graphics
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
                     const core::span<const char*>&        enabledExtensions  = {},
                     const core::span<const char*>&        enabledLayers      = {},
                     VkInstanceCreateFlags                 flags              = 0u);
  ~Instance();
  core::vector<core::Shared_Ptr<Physical_Device>> getPhysicalDevices();
};

}
