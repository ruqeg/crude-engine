#include "image.hpp"
#include "device.hpp"

namespace crude_vulkan_01 
{

Image_Create_Info::Image_Create_Info(std::shared_ptr<const Device>  device,
                                     VkImage                        handle,
                                     VkFormat                       format,
                                     VkColorSpaceKHR                colorSpace,
                                     VkExtent2D                     extent,
                                     VkImageUsageFlags              usage,
                                     VkImageType                    type)
  :
  device(device),
  handle(handle),
  format(format),
  colorSpace(colorSpace),
  extent(extent),
  usage(usage),
  type(type)
{}
  
Image::Image(const Image_Create_Info& createInfo)
{
  m_handle      = createInfo.handle;
  m_device      = createInfo.device;
  m_format      = createInfo.format;
  m_colorSpace  = createInfo.colorSpace;
  m_extent      = createInfo.extent;
  m_usage       = createInfo.usage;
  m_type        = createInfo.type;
}
  
Image::~Image()
{
  vkDestroyImage(CRUDE_VULKAN_01_HANDLE(m_device), m_handle, nullptr);
}
  
VkImageType Image::getType() const
{
  return m_type;
}

}
