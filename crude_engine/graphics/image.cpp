#include <vulkan/vulkan.hpp>

module crude.graphics.vulkan.image;

import crude.graphics.vulkan.device;
import crude.graphics.vulkan.vulkan_utils;

namespace crude::graphics
{

Image::Image(core::Shared_Ptr<const Device>  device,
             VkImage                         handle,
             VkFormat                        format,
             VkExtent3D                      extent,
             VkImageUsageFlags               usage,
             VkImageType                     type)
  :
  m_device(device)
{
  m_handle      = handle;
  m_format      = format;
  m_extent      = extent;
  m_usage       = usage;
  m_type        = type;
  m_layout      = VK_IMAGE_LAYOUT_UNDEFINED;
}

Image::Image(core::Shared_Ptr<const Device>  device,
             VkImageCreateFlags              flags,
             VkFormat                        format,
             const VkExtent2D&               extent,
             core::uint32                    mipLevels,
             core::uint32                    arrayLayers,
             VkSampleCountFlagBits           samples,
             VkImageTiling                   tiling,
             VkImageUsageFlags               usage,
             VkSharingMode                   sharingMode,
             VkImageLayout                   initialLayout)
  :
  m_device(device)
{
  m_format         = format;
  m_usage          = usage;
  m_extent.width   = extent.width;
  m_extent.height  = extent.height;
  m_extent.depth   = 1u;
  m_type           = VK_IMAGE_TYPE_2D;
  m_layout         = initialLayout;

  VkImageCreateInfo vkImageInfo{};
  vkImageInfo.sType                  = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  vkImageInfo.pNext                  = nullptr;
  vkImageInfo.queueFamilyIndexCount  = 0u; // !TODO
  vkImageInfo.pQueueFamilyIndices    = nullptr; // !TODO

  vkImageInfo.flags                  = flags;
  vkImageInfo.imageType              = m_type;
  vkImageInfo.format                 = m_format;
  vkImageInfo.extent                 = m_extent;
  vkImageInfo.mipLevels              = mipLevels;
  vkImageInfo.arrayLayers            = arrayLayers;
  vkImageInfo.samples                = samples;
  vkImageInfo.tiling                 = tiling;
  vkImageInfo.usage                  = m_usage;
  vkImageInfo.sharingMode            = sharingMode;
  vkImageInfo.initialLayout          = m_layout;

  VkResult result = vkCreateImage(m_device->getHandle(), &vkImageInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create 2d image");
}
  
Image::~Image()
{
  if (m_handle != VK_NULL_HANDLE)
  {
    vkDestroyImage(m_device->getHandle(), m_handle, getPVkAllocationCallbacks());
  }
}

void Image::setLayout(VkImageLayout layout)
{
  m_layout = layout;
}
  
VkImageType Image::getType() const
{
  return m_type;
}

VkFormat Image::getFormat() const
{
  return m_format;
}

VkImageLayout Image::getLayout() const
{
  return m_layout;
}

VkMemoryRequirements Image::getMemoryRequirements() const
{
  VkMemoryRequirements memRequirements;
  vkGetImageMemoryRequirements(m_device->getHandle(), m_handle, &memRequirements);
  return memRequirements;
}

}
