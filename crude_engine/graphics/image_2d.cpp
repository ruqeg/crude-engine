#include <vulkan/vulkan.hpp>

module crude.graphics.image_2d;

import crude.graphics.command_buffer;

namespace crude::graphics
{

Image_2D::Image_2D(core::shared_ptr<const Device>  device,
                   VkFormat                        format,
                   const VkExtent3D&               extent,
                   core::uint32                    mipLevels,
                   VkSharingMode                   sharingMode)
  :
  Image(device, VK_IMAGE_TYPE_2D, format, extent, mipLevels, 
    1u, VK_SAMPLE_COUNT_1_BIT, 0u,
    VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
    VK_IMAGE_TILING_OPTIMAL,
    sharingMode)
{}

Image_2D::Image_2D(core::shared_ptr<Command_Buffer>  commandBuffer,
                   VkFormat                          format,
                   const Mip_Data&                   mipData,
                   VkSharingMode                     sharingMode)
  :
  Image_2D(commandBuffer->getDevice(), format, mipData.extent, 1u, sharingMode)
{
  stagedUpload(commandBuffer, mipData, 0u, 1u, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);
}

Image_2D::Image_2D(core::shared_ptr<const Device>  device,
                   VkFormat                        format,
                   const VkExtent2D&               extent,
                   core::uint32                    mipLevels,
                   core::uint32                    arrayLayers,
                   VkSampleCountFlagBits           samples,
                   VkImageCreateFlags              flags,
                   VkImageUsageFlags               usage,
                   VkImageTiling                   tiling,
                   VkSharingMode                   sharingMode)
  :
  Image(device, VK_IMAGE_TYPE_2D, format, VkExtent3D{ extent.width, extent.height, 1 }, mipLevels,
    arrayLayers, samples, flags, usage, tiling, sharingMode)
{}

Image_2D::Image_2D(core::shared_ptr<const Device>  device,
                   VkImage                         handle,
                   VkFormat                        format,
                   const VkExtent2D&               extent)
  :
  Image(device, handle, VK_IMAGE_TYPE_2D, format, VkExtent3D{ extent.width, extent.height, 1 })
{}


}