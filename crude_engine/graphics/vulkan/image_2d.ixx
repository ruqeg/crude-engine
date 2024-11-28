module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.image_2d;

export import crude.gfx.vk.image;

export namespace crude::gfx::vk
{

class Image_2D : public Image
{
public:
  explicit Image_2D(core::shared_ptr<const Device>  device,
                    VkFormat                        format,
                    const VkExtent3D&               extent,
                    core::uint32                    mipLevelsCount,
                    VkSharingMode                   sharingMode);
  explicit Image_2D(core::shared_ptr<Command_Buffer>  commandBuffer,
                    VkFormat                          format,
                    const Mip_Data&                   mipData,
                    VkSharingMode                     sharingMode);
  explicit Image_2D(core::shared_ptr<Command_Buffer>  commandBuffer,
                    VkFormat                          format,
                    const Mip_Data&                   mipData,
                    core::uint32                      mipLevelsCount,
                    VkSharingMode                     sharingMode);
protected:
  explicit Image_2D(core::shared_ptr<const Device>  device,
                    VkFormat                        format,
                    const VkExtent2D&               extent,
                    core::uint32                    mipLevelsCount,
                    core::uint32                    arrayLayersCount,
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