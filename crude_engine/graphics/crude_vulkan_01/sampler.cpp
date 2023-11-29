#include "sampler.hpp"
#include "device.hpp"

namespace crude_vulkan_01
{
  
Sampler_Create_Info::Sampler_Create_Info(std::shared_ptr<const Device>  device,
                                         VkFilter                       magFilter,
                                         VkFilter                       minFilter,
                                         VkSamplerMipmapMode            mipmapMode,
                                         VkSamplerAddressMode           addressModeU,
                                         VkSamplerAddressMode           addressModeV,
                                         VkSamplerAddressMode           addressModeW,
                                         float32                        mipLodBias,
                                         VkBool32                       anisotropyEnable,
                                         float32                        maxAnisotropy,
                                         VkBool32                       compareEnable,
                                         VkCompareOp                    compareOp,
                                         float32                        minLod,
                                         float32                        maxLod,
                                         VkBorderColor                  borderColor,
                                         VkBool32                       unnormalizedCoordinates)
  :
  device(device),
  magFilter(magFilter),
  minFilter(minFilter),
  mipmapMode(mipmapMode),
  addressModeU(addressModeU),
  addressModeV(addressModeV),
  addressModeW(addressModeW),
  mipLodBias(mipLodBias),
  anisotropyEnable(anisotropyEnable),
  maxAnisotropy(maxAnisotropy),
  compareEnable(compareEnable),
  compareOp(compareOp),
  minLod(minLod),
  maxLod(maxLod),
  borderColor(borderColor),
  unnormalizedCoordinates(unnormalizedCoordinates)
{}

Sampler::Sampler(const Sampler_Create_Info& createInfo)
{
  m_device = createInfo.device;

  VkSamplerCreateInfo vkCreateInfo{};
  vkCreateInfo.sType                    = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  vkCreateInfo.pNext                    = nullptr;
  vkCreateInfo.flags                    = 0u;
  vkCreateInfo.magFilter                = createInfo.magFilter;
  vkCreateInfo.minFilter                = createInfo.minFilter;
  vkCreateInfo.mipmapMode               = createInfo.mipmapMode;
  vkCreateInfo.addressModeU             = createInfo.addressModeU;
  vkCreateInfo.addressModeV             = createInfo.addressModeV;
  vkCreateInfo.addressModeW             = createInfo.addressModeW;
  vkCreateInfo.mipLodBias               = createInfo.mipLodBias;
  vkCreateInfo.anisotropyEnable         = createInfo.anisotropyEnable;
  vkCreateInfo.maxAnisotropy            = createInfo.maxAnisotropy;
  vkCreateInfo.compareEnable            = createInfo.compareEnable;
  vkCreateInfo.compareOp                = createInfo.compareOp;
  vkCreateInfo.minLod                   = createInfo.minLod;
  vkCreateInfo.maxLod                   = createInfo.maxLod;
  vkCreateInfo.borderColor              = createInfo.borderColor;
  vkCreateInfo.unnormalizedCoordinates  = createInfo.unnormalizedCoordinates;

  VkResult result = vkCreateSampler(CRUDE_VULKAN_01_HANDLE(m_device), &vkCreateInfo, nullptr, &m_handle);
  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to create sampler");
}

Sampler::~Sampler()
{
  vkDestroySampler(CRUDE_VULKAN_01_HANDLE(m_device), m_handle, nullptr);
}

}
