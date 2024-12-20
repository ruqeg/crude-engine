#include <vulkan/vulkan.hpp>

module crude.gfx.vk.image_cube;

import crude.gfx.vk.image;

namespace crude::gfx::vk
{

Image_Cube::Image_Cube(core::shared_ptr<const Device>  device,
                       VkFormat                        format,
                       core::uint32                    dimension,
                       core::uint32                    mipLevelsCount,
                       VkSharingMode                   sharingMode)
  : Image(device, VK_IMAGE_TYPE_2D, format, VkExtent3D{ dimension, dimension, 1u }, mipLevelsCount,
      6u, 
      VK_SAMPLE_COUNT_1_BIT,
      VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
      VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
      VK_IMAGE_TILING_OPTIMAL,
      sharingMode)
{}
  
Image_Cube::Image_Cube(core::shared_ptr<const Device>  device,
                       VkFormat                        format,
                       core::uint32                    dimension,
                       core::uint32                    mipLevelsCount,
                       core::uint32                    arrayCount,
                       VkSampleCountFlagBits           samples,
                       VkImageCreateFlags              flags,
                       VkImageUsageFlags               usage,
                       VkImageTiling                   tiling,
                       VkSharingMode                   sharingMode)
  : Image(device, VK_IMAGE_TYPE_2D, format, VkExtent3D{ dimension, dimension, 1 }, mipLevelsCount,
    arrayCount * 6u,
    samples,
    flags | VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT, 
    usage, tiling, sharingMode)
{}

}