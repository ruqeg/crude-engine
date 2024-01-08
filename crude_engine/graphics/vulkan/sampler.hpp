#pragma once

#include <core/data_structures/shared_ptr.hpp>
#include <graphics/vulkan/include_vulkan.hpp>
#include <graphics/vulkan/object.hpp>

namespace crude_engine
{

class Device;

class Sampler : public TObject<VkSampler>
{
public:
  explicit Sampler(Shared_Ptr<const Device>  device,
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
                   VkBool32                  unnormalizedCoordinates);
  ~Sampler();
private:
  Shared_Ptr<const Device>  m_device;
};

}
