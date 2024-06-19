module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.image_attachment;

export import crude.graphics.image_2d;

export namespace crude::graphics
{

class Depth_Stencil_Attachment : public Image_2D
{
public:
  explicit Depth_Stencil_Attachment(core::shared_ptr<Device>  device,
                                    VkFormat                  depthStencilFormat,
                                    const VkExtent2D&         extent,
                                    core::uint32              mipLevels,
                                    VkSampleCountFlagBits     samples,
                                    VkSharingMode             sharingMode)
    :
    Image_2D(device, depthStencilFormat, extent, mipLevels, 
      1u, samples, 0u, 
      VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_TILING_OPTIMAL, sharingMode)
  {}
};

}