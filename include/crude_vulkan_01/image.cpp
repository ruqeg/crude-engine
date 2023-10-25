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
                                 VkImageUsageFlags             usage)
  :
  device(device),
  handle(handle),
  format(format),
  colorSpace(colorSpace),
  extent(extent),
  usage(usage)
{}
  
Image::Image(const ImageCreateInfo& createInfo)
{
  m_handle      = createInfo.handle;
  m_device      = createInfo.device;
  m_format      = createInfo.format;
  m_colorSpace  = createInfo.colorSpace;
  m_extent      = createInfo.extent;
  m_usage       = createInfo.usage;
}
  
Image::~Image()
{
  vkDestroyImage(
    CRUDE_VULKAN_01_HANDLE(m_device), 
    m_handle, 
    nullptr);
}

}
