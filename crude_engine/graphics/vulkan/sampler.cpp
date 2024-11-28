#include <vulkan/vulkan.hpp>

module crude.gfx.vk.sampler;

import crude.gfx.vk.device;
import crude.gfx.vk.vulkan_utils;

namespace crude::gfx::vk
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
