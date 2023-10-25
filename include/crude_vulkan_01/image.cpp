#include "image.hpp"
#include "core.hpp"
#include "device.hpp"
#include <vulkan/vulkan_core.h>

namespace crude_vulkan_01 
{

ImageCreateInfo::ImageCreateInfo(std::shared_ptr<const Device> device,
                                 VkImage                       handle,
                                 VkFormat                      format,
                                 VkColorSpaceKHR               colorSpace,
                                 VkExtent2D                    extent,
                                 VkImageUsageFlags             usage,
                                 VkImageType                   type)
  :
  device(device),
  handle(handle),
  format(format),
  colorSpace(colorSpace),
  extent(extent),
  usage(usage),
  type(type)
{}
  
Image::Image(const ImageCreateInfo& createInfo)
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
  vkDestroyImage(
    CRUDE_VULKAN_01_HANDLE(m_device), 
    m_handle, 
    nullptr);
}
  
VkImageType Image::getType() const
{
  return m_type;
}

}
