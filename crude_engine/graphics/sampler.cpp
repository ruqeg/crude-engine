#include <vulkan/vulkan.hpp>

module crude.graphics.sampler;

import crude.graphics.device;
import crude.graphics.vulkan_utils;

namespace crude::graphics
{
  
Sampler::Sampler(core::Shared_Ptr<const Device>  device,
                 VkFilter                        magFilter,
                 VkFilter                        minFilter,
                 VkSamplerMipmapMode             mipmapMode,
                 VkSamplerAddressMode            addressModeU,
                 VkSamplerAddressMode            addressModeV,
                 VkSamplerAddressMode            addressModeW,
                 core::float32                   mipLodBias,
                 VkBool32                        anisotropyEnable,
                 core::float32                   maxAnisotropy,
                 VkBool32                        compareEnable,
                 VkCompareOp                     compareOp,
                 core::float32                   minLod,
                 core::float32                   maxLod,
                 VkBorderColor                   borderColor,
                 VkBool32                        unnormalizedCoordinates)
  :
  m_device(device)
{
  VkSamplerCreateInfo vkCreateInfo{};
  vkCreateInfo.sType                    = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  vkCreateInfo.pNext                    = nullptr;
  vkCreateInfo.flags                    = 0u;
  vkCreateInfo.magFilter                = magFilter;
  vkCreateInfo.minFilter                = minFilter;
  vkCreateInfo.mipmapMode               = mipmapMode;
  vkCreateInfo.addressModeU             = addressModeU;
  vkCreateInfo.addressModeV             = addressModeV;
  vkCreateInfo.addressModeW             = addressModeW;
  vkCreateInfo.mipLodBias               = mipLodBias;
  vkCreateInfo.anisotropyEnable         = anisotropyEnable;
  vkCreateInfo.maxAnisotropy            = maxAnisotropy;
  vkCreateInfo.compareEnable            = compareEnable;
  vkCreateInfo.compareOp                = compareOp;
  vkCreateInfo.minLod                   = minLod;
  vkCreateInfo.maxLod                   = maxLod;
  vkCreateInfo.borderColor              = borderColor;
  vkCreateInfo.unnormalizedCoordinates  = unnormalizedCoordinates;

  VkResult result = vkCreateSampler(m_device->getHandle(), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create sampler");
}

Sampler::~Sampler()
{
  vkDestroySampler(m_device->getHandle(), m_handle, getPVkAllocationCallbacks());
}

}
