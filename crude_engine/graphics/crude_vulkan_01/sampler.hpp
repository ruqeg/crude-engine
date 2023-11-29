#pragma once

#include "../../core/core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"

namespace crude_vulkan_01
{

class Device;

struct Sampler_Create_Info
{
  std::shared_ptr<const Device>  device;
  VkFilter                       magFilter;
  VkFilter                       minFilter;
  VkSamplerMipmapMode            mipmapMode;
  VkSamplerAddressMode           addressModeU;
  VkSamplerAddressMode           addressModeV;
  VkSamplerAddressMode           addressModeW;
  float32                        mipLodBias;
  VkBool32                       anisotropyEnable;
  float32                        maxAnisotropy;
  VkBool32                       compareEnable;
  VkCompareOp                    compareOp;
  float32                        minLod;
  float32                        maxLod;
  VkBorderColor                  borderColor;
  VkBool32                       unnormalizedCoordinates;

  explicit Sampler_Create_Info(std::shared_ptr<const Device>  device,
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
                               VkBool32                       unnormalizedCoordinates);
};

class Sampler : public TObject<VkSampler>
{
public:
  explicit Sampler(const Sampler_Create_Info& createInfo);
  ~Sampler();
private:
  std::shared_ptr<const Device>  m_device;
};

}
