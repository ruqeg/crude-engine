#include <vulkan/vulkan.h>

module crude.gfx.vk.extension;

import crude.gfx.vk.device;
import crude.gfx.vk.instance;

namespace crude::gfx::vk
{

PFN_vkVoidFunction getInstanceProcAddr(core::shared_ptr<const Instance> instance, const char* procName)
{
  return vkGetInstanceProcAddr(instance->getHandle(), procName);
}

PFN_vkVoidFunction gettDeviceProcAddr(core::shared_ptr<const Device> device, const char* procName)
{
  return vkGetDeviceProcAddr(device->getHandle(), procName);
}

}