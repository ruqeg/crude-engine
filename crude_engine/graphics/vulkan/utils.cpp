module;

#include <vulkan/vulkan.h>

module crude.gfx.vk.utils;

import crude.gfx.vk.buffer;
import crude.gfx.vk.constants;

namespace crude::gfx::vk
{

VkDeviceOrHostAddressConstKHR getAddress(const core::shared_ptr<const Buffer> buffer)
{
  VkDeviceOrHostAddressConstKHR addr{};
  addr.deviceAddress = buffer ? buffer->getDeviceAddress() : cDeviceAddressNull;
  return addr;
}

}
