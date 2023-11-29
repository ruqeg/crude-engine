#pragma once

#include "../../core/core.hpp"
#include "object.hpp"
#include "application.hpp"
#include <vector>

namespace crude_vulkan_01
{

class Physical_Device;

struct Instance_Create_Info
{
  Application                           application;
#ifdef VK_EXT_debug_utils
  PFN_vkDebugUtilsMessengerCallbackEXT  debugUtilsCallback;
#endif // VK_EXT_debug_utils
  VkInstanceCreateFlags                 flags;
  std::vector<const char*>              enabledExtensions;
  std::vector<const char*>              enabledLayers;

  explicit Instance_Create_Info(
#ifdef VK_EXT_debug_utils
                              PFN_vkDebugUtilsMessengerCallbackEXT  debugUtilsCallback,
#endif // VK_EXT_debug_utils
                              const Application&                    application        = Application(),
                              const std::vector<const char*>&       enabledExtensions  = {},
                              const std::vector<const char*>&       enabledLayers      = {},
                              VkInstanceCreateFlags                 flags              = 0u);
};

class Instance : public TObject<VkInstance>
{
public:
  explicit Instance(const Instance_Create_Info& createInfo);
  ~Instance();
  std::vector<std::shared_ptr<Physical_Device>> getPhysicalDevices();
};

}
