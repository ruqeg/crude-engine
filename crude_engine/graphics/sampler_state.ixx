module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.sampler_state;

import crude.core.std_containers_heap;
import crude.graphics.vulkan_object;

export namespace crude::graphics
{

class Sampler_State : public VkSamplerCreateInfo
{
public:
  constexpr explicit Sampler_State(VkFilter              magFilter, 
                                   VkFilter              minFilter,
                                   VkSamplerMipmapMode   mipmapMode, 
                                   VkSamplerAddressMode  addressMode) noexcept
  {
    this->sType                    = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    this->pNext                    = nullptr;
    this->flags                    = 0;
    this->magFilter                = magFilter;
    this->minFilter                = minFilter;
    this->mipmapMode               = mipmapMode;
    this->addressModeU             = addressMode;
    this->addressModeV             = addressMode;
    this->addressModeW             = addressMode;
    this->mipLodBias               = 0.f;
    this->anisotropyEnable         = VK_FALSE;
    this->maxAnisotropy            = 1.f;
    this->compareEnable            = VK_FALSE;
    this->compareOp                = VK_COMPARE_OP_NEVER;
    this->minLod                   = 0.f;
    this->maxLod                   = VK_LOD_CLAMP_NONE;
    this->borderColor              = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
    this->unnormalizedCoordinates  = VK_FALSE;
  }
};

namespace csamlper_state
{

constexpr Sampler_State gMagMinMipNearestRepeat(VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT);
constexpr Sampler_State gMagMinMipLinearRepeat(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT);

}

}
