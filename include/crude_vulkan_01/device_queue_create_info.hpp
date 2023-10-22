#pragma once

#include "core.hpp"
#include "object.hpp"
#include "include_vulkan.hpp"
#include <vector>
#include <vulkan/vulkan_core.h>

namespace crude_vulkan_01 
{

class Physical_Device;

class DeviceQueueCreateInfo final : public VkDeviceQueueCreateInfo
{
public:
  explicit DeviceQueueCreateInfo(uint32                       queueFamilyIndex,
                                 const std::vector<float32>&  queuePriorities = {1.f});
};

}
