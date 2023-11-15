#include "image.hpp"
#include "device.hpp"

namespace crude_vulkan_01 
{

Image_From_Handle_Create_Info::Image_From_Handle_Create_Info(std::shared_ptr<const Device>  device,
                                                             VkImage                        handle,
                                                             VkFormat                       format,
                                                             VkExtent3D                     extent,
                                                             VkImageUsageFlags              usage,
                                                             VkImageType                    type)
  :
  device(device),
  handle(handle),
  format(format),
  extent(extent),
  usage(usage),
  type(type)
{}

Image_2D_Create_Info::Image_2D_Create_Info(std::shared_ptr<const Device>  device,
                                           VkImageCreateFlags             flags,
                                           VkFormat                       format,
                                           const VkExtent2D&              extent,
                                           uint32                         mipLevels,
                                           uint32                         arrayLayers,
                                           VkSampleCountFlagBits          samples,
                                           VkImageTiling                  tiling,
                                           VkImageUsageFlags              usage,
                                           VkSharingMode                  sharingMode,
                                           VkImageLayout                  initialLayout)
  :
  device(device),
  flags(flags),
  format(format),
  extent(extent),
  mipLevels(mipLevels),
  arrayLayers(arrayLayers),
  samples(samples),
  tiling(tiling),
  usage(usage),
  sharingMode(sharingMode),
  initialLayout(initialLayout)
{}
  
Image::Image(const Image_From_Handle_Create_Info& createInfo)
{
  m_handle      = createInfo.handle;
  m_device      = createInfo.device;
  m_format      = createInfo.format;
  m_extent      = createInfo.extent;
  m_usage       = createInfo.usage;
  m_type        = createInfo.type;
  m_layout      = VK_IMAGE_LAYOUT_UNDEFINED;
}

Image::Image(const Image_2D_Create_Info& createInfo)
{
  m_device         = createInfo.device;
  m_format         = createInfo.format;
  m_usage          = createInfo.usage;
  m_extent.width   = createInfo.extent.width;
  m_extent.height  = createInfo.extent.height;
  m_extent.depth   = 1u;
  m_type           = VK_IMAGE_TYPE_2D;
  m_layout         = createInfo.initialLayout;

  VkImageCreateInfo vkImageInfo{};
  vkImageInfo.sType                  = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  vkImageInfo.pNext                  = nullptr;
  vkImageInfo.queueFamilyIndexCount  = 0u; // !TODO
  vkImageInfo.pQueueFamilyIndices    = nullptr; // !TODO

  vkImageInfo.flags                  = createInfo.flags;
  vkImageInfo.imageType              = m_type;
  vkImageInfo.format                 = m_format;
  vkImageInfo.extent                 = m_extent;
  vkImageInfo.mipLevels              = createInfo.mipLevels;
  vkImageInfo.arrayLayers            = createInfo.arrayLayers;
  vkImageInfo.samples                = createInfo.samples;
  vkImageInfo.tiling                 = createInfo.tiling;
  vkImageInfo.usage                  = m_usage;
  vkImageInfo.sharingMode            = createInfo.sharingMode;
  vkImageInfo.initialLayout          = m_layout;

  VkResult result = vkCreateImage(CRUDE_VULKAN_01_HANDLE(m_device), &vkImageInfo, nullptr, &m_handle);
  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to create 2d image");
}
  
Image::~Image()
{
  if (m_handle != VK_NULL_HANDLE)  vkDestroyImage(CRUDE_VULKAN_01_HANDLE(m_device), m_handle, nullptr);
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
  vkGetImageMemoryRequirements(CRUDE_VULKAN_01_HANDLE(m_device), m_handle, &memRequirements);
  return memRequirements;
}

}
