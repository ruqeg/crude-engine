#include <vulkan/vulkan.h>

module crude.graphics.extension;

import crude.graphics.device;
import crude.graphics.instance;

namespace crude::graphics
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