module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.image_attachment;

export import crude.graphics.image_2d;

export namespace crude::graphics
{

class Depth_Stencil_Attachment : public Image_2D
{
public:
  explicit Depth_Stencil_Attachment(core::shared_ptr<const Device>  device,
                                    VkFormat                        depthStencilFormat,
                                    const VkExtent2D&               extent,
                                    core::uint32                    mipLevelsCount,
                                    VkSampleCountFlagBits           samples,
                                    VkSharingMode                   sharingMode = VK_SHARING_MODE_EXCLUSIVE)
    :
    Image_2D(device, depthStencilFormat, extent, mipLevelsCount,
      1u, samples, 0u, 
      VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_TILING_OPTIMAL, sharingMode)
  {}
};

class Color_Attachment : public Image_2D
{
public:
  explicit Color_Attachment(core::shared_ptr<const Device>  device,
                            VkFormat                        format,
                            const VkExtent2D&               extent,
                            core::uint32                    mipLevelsCount,
                            VkSampleCountFlagBits           samples,
                            VkSharingMode                   sharingMode = VK_SHARING_MODE_EXCLUSIVE)
    :
    Image_2D(device, format, extent, mipLevelsCount,
      1u, samples, 0u, 
      VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_IMAGE_TILING_OPTIMAL, sharingMode)
  {}
};

}