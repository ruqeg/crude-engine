module;

#include <vulkan/vulkan.h>

export module crude.graphics.application;

import crude.core.alias;

export namespace crude::graphics
{

struct Application final : public VkApplicationInfo
{
  explicit Application(const char*   pApplicationName   = "default application name",
                       core::uint32  applicationVersion = VK_MAKE_VERSION(1, 0, 0),
                       const char*   pEngineName        = "default engine name",
                       core::uint32  engineVersion      = VK_MAKE_VERSION(1, 0, 0),
                       core::uint32  apiVersion         = VK_API_VERSION_1_0);
};

}
