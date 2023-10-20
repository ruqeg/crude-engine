#pragma once

#include "core.hpp"
#include "include_vulkan.hpp"

namespace crude_vulkan_01 
{

struct Application final : public VkApplicationInfo {
  explicit Application(const char8*  pApplicationName   = "default application name",
                       uint32        applicationVersion = VK_MAKE_VERSION(1, 0, 0),
                       const char8*  pEngineName        = "default engine name",
                       uint32        engineVersion      = VK_MAKE_VERSION(1, 0, 0),
                       uint32        apiVersion         = VK_API_VERSION_1_0);
};

}
