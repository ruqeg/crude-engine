module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.sampler;

import crude_engine.core.shared_ptr;
import crude_engine.graphics.vulkan.vulkan_object;

export namespace crude_engine
{

class Device;

class Sampler : public Vulkan_Object<VkSampler>
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
