#pragma once

#include <graphics/vulkan/include_vulkan.hpp>
#include <core/shared_ptr.hpp>
#include <core/std_containers.hpp>
#include <graphics/vulkan/application.hpp>
#include <graphics/vulkan/object.hpp>

namespace crude_engine
{

class Physical_Device;


class Instance : public TObject<VkInstance>
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
