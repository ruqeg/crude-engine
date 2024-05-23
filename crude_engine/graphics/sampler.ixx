module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.sampler;

import crude.core.shared_ptr;
import crude.graphics.vulkan_object;

export namespace crude::graphics
{

class Device;

class Sampler : public Vulkan_Object<VkSampler>
{
public:
  explicit Sampler(core::Shared_Ptr<const Device>  device,
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
                   VkBool32                        unnormalizedCoordinates);
  ~Sampler();
private:
  core::Shared_Ptr<const Device>  m_device;
};

}
