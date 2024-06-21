#include <vulkan/vulkan.hpp>

module crude.graphics.sampler;

import crude.graphics.device;
import crude.graphics.vulkan_utils;

namespace crude::graphics
{
  
Sampler::Sampler(core::shared_ptr<const Device>  device,
                 const Sampler_State&            samplerState)
  :
  m_device(device)
{
  VkResult result = vkCreateSampler(m_device->getHandle(), &samplerState, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create sampler");
}

Sampler::~Sampler()
{
  vkDestroySampler(m_device->getHandle(), m_handle, getPVkAllocationCallbacks());
}

}
