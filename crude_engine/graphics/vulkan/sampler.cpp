#include <graphics/vulkan/sampler.hpp>
#include <graphics/vulkan/device.hpp>

namespace crude_engine
{
  
Sampler::Sampler(Shared_Ptr<const Device>  device,
                 VkFilter                  magFilter,
                 VkFilter                  minFilter,
                 VkSamplerMipmapMode       mipmapMode,
                 VkSamplerAddressMode      addressModeU,
                 VkSamplerAddressMode      addressModeV,
                 VkSamplerAddressMode      addressModeW,
                 float32                   mipLodBias,
                 VkBool32                  anisotropyEnable,
                 float32                   maxAnisotropy,
                 VkBool32                  compareEnable,
                 VkCompareOp               compareOp,
                 float32                   minLod,
                 float32                   maxLod,
                 VkBorderColor             borderColor,
                 VkBool32                  unnormalizedCoordinates)
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

  VkResult result = vkCreateSampler(CRUDE_OBJECT_HANDLE(m_device), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  CRUDE_VULKAN_HANDLE_RESULT(result, "failed to create sampler");
}

Sampler::~Sampler()
{
  vkDestroySampler(CRUDE_OBJECT_HANDLE(m_device), m_handle, getPVkAllocationCallbacks());
}

}
