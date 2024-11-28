module;

#include <vulkan/vulkan.hpp>
#include <memory>

export module crude.gfx.vk.instance;

export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.gfx.vk.vulkan_object;
export import crude.gfx.vk.application;

export namespace crude::gfx::vk
{

class Physical_Device;

class Instance : public Vulkan_Object<VkInstance>,
  public std::enable_shared_from_this<Instance>
{
public:
  explicit Instance(
#ifdef VK_EXT_debug_utils
                    PFN_vkDebugUtilsMessengerCallbackEXT   debugUtilsCallback,
#endif // VK_EXT_debug_utils
                    const Application&                     application        = Application(Application::Initialize()),
                    core::span<const char* const>          enabledExtensions  = {},
                    core::span<const char* const>          enabledLayers      = {},
                    VkInstanceCreateFlags                  flags              = 0u);
  ~Instance();
  core::vector<core::shared_ptr<Physical_Device>> getPhysicalDevices();
};

}
