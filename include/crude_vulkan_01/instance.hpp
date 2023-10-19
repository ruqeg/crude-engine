#pragma once

#include "core/core.hpp"
#include "object.hpp"
#include "include_vulkan.hpp"
#include "application.hpp"
#include <vector>

namespace crude_vulkan_01
{

struct InstanceCreateInfo {
  Application               application;
#ifdef VK_EXT_debug_utils
  PFN_vkDebugUtilsMessengerCallbackEXT debugUtilsCallback;
#endif // VK_EXT_debug_utils
  VkInstanceCreateFlags     flags;
  std::vector<const char*>  enabledExtensions;
  std::vector<const char*>  enabledLayers;

  InstanceCreateInfo() = default;
  InstanceCreateInfo(const Application&               application,
#ifdef VK_EXT_debug_utils
                     PFN_vkDebugUtilsMessengerCallbackEXT debugUtilsCallback,
#endif // VK_EXT_debug_utils
                     VkInstanceCreateFlags            flags,
                     const std::vector<const char*>&  enabledExtensions,
                     const std::vector<const char*>&  enabledLayers);
};

class Instance : public TObject<VkInstance>
{
public:
  explicit Instance(const InstanceCreateInfo& createInfo);
  ~Instance();
private:
  
};

}
