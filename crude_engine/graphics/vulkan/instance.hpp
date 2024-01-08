#pragma once

#include <graphics/vulkan/include_vulkan.hpp>
#include <core/data_structures/shared_ptr.hpp>
#include <core/data_structures/array_dynamic.hpp>
#include <core/data_structures/array_unsafe.hpp>
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
                    PFN_vkDebugUtilsMessengerCallbackEXT  debugUtilsCallback,
#endif // VK_EXT_debug_utils
                     const Application&                    application        = Application(),
                     const Array_Unsafe<const char*>&      enabledExtensions  = {},
                     const Array_Unsafe<const char*>&      enabledLayers      = {},
                     VkInstanceCreateFlags                 flags              = 0u);
  ~Instance();
  Array_Dynamic<Shared_Ptr<Physical_Device>> getPhysicalDevices();
};

}
