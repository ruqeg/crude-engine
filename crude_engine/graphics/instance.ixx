module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.instance;

export import crude.core.shared_ptr;
export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.graphics.vulkan_object;
export import crude.graphics.application;

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
