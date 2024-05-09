module;

#include <vulkan/vulkan.h>

export module crude_engine.graphics.vulkan.application;

import crude_engine.core.alias;

export namespace crude_engine
{

struct Application final : public VkApplicationInfo
{
  explicit Application(const char*  pApplicationName   = "default application name",
                       uint32       applicationVersion = VK_MAKE_VERSION(1, 0, 0),
                       const char*  pEngineName        = "default engine name",
                       uint32       engineVersion      = VK_MAKE_VERSION(1, 0, 0),
                       uint32       apiVersion         = VK_API_VERSION_1_0);
};

}
