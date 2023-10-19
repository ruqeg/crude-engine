#pragma once

#include "core/types.hpp"
#include "include_vulkan.hpp"

namespace crude_vulkan_01 
{

struct Application final : public VkApplicationInfo {
public:
  Application() = default;
  Application(const char8*  pApplicationName,
              uint32        applicationVersion,
              const char8*  pEngineName,
              uint32        engineVersion,
              uint32        apiVersion);
};

}
