#include <vulkan/vulkan.hpp>

module crude.gfx.vk.image_2d;

import crude.gfx.vk.command_buffer;

namespace crude::gfx::vk
{

Image_2D::Image_2D(core::shared_ptr<const Device>  device,
                   VkFormat                        format,
                   const VkExtent3D&               extent,
                   core::uint32                    mipLevelsCount,
                   VkSharingMode                   sharingMode)
  :
  Image(device, VK_IMAGE_TYPE_2D, format, extent, mipLevelsCount,
    1u, VK_SAMPLE_COUNT_1_BIT, 0u,
    VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
    VK_IMAGE_TILING_OPTIMAL,
    sharingMode)
{}

Image_2D::Image_2D(core::shared_ptr<Command_Buffer>  commandBuffer,
                   VkFormat                          format,
                   const Mip_Data&                   mipData,
                   VkSharingMode                     sharingMode)
  :
  Image_2D(commandBuffer, format, mipData, 1u, sharingMode)
{}


Image_2D::Image_2D(core::shared_ptr<Command_Buffer>  commandBuffer,
                   VkFormat                          format,
                   const Mip_Data&                   mipData,
                   core::uint32                      mipLevelsCount,
                   VkSharingMode                     sharingMode)
  :
  Image_2D(commandBuffer->getDevice(), format, mipData.extent, mipLevelsCount, sharingMode)
{
  stagedUpload(commandBuffer, mipData, 0u, 1u, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);
}

Image_2D::Image_2D(core::shared_ptr<const Device>  device,
                   VkFormat                        format,
                   const VkExtent2D&               extent,
                   core::uint32                    mipLevelsCount,
                   core::uint32                    arrayLayersCount,
                   VkSampleCountFlagBits           samples,
                   VkImageCreateFlags              flags,
                   VkImageUsageFlags               usage,
                   VkImageTiling                   tiling,
                   VkSharingMode                   sharingMode)
  :
  Image(device, VK_IMAGE_TYPE_2D, format, VkExtent3D{ extent.width, extent.height, 1 }, mipLevelsCount,
    arrayLayersCount, samples, flags, usage, tiling, sharingMode)
{}

Image_2D::Image_2D(core::shared_ptr<const Device>  device,
                   VkImage                         handle,
                   VkFormat                        format,
                   const VkExtent2D&               extent)
  :
  Image(device, handle, VK_IMAGE_TYPE_2D, format, VkExtent3D{ extent.width, extent.height, 1 })
{}


}