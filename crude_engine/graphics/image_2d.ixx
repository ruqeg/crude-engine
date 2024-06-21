module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.image_2d;

export import crude.graphics.image;

export namespace crude::graphics
{

class Image_2D : public Image
{
public:
  explicit Image_2D(core::shared_ptr<const Device>  device,
                    VkFormat                        format,
                    const VkExtent3D&               extent,
                    core::uint32                    mipLevels,
                    VkSharingMode                   sharingMode);
  explicit Image_2D(core::shared_ptr<Command_Buffer>  commandBuffer,
                    VkFormat                          format,
                    const Mip_Data&                   mipData,
                    VkSharingMode                     sharingMode);
protected:
  explicit Image_2D(core::shared_ptr<const Device>  device,
                    VkFormat                        format,
                    const VkExtent2D&               extent,
                    core::uint32                    mipLevels,
                    core::uint32                    arrayLayers,
                    VkSampleCountFlagBits           samples,
                    VkImageCreateFlags              flags,
                    VkImageUsageFlags               usage,
                    VkImageTiling                   tiling,
                    VkSharingMode                   sharingMode);
  explicit Image_2D(core::shared_ptr<const Device>  device,
                    VkImage                         handle,
                    VkFormat                        format,
                    const VkExtent2D&               extent);
};

}