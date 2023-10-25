#pragma once

#include "core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"

namespace crude_vulkan_01 
{

class Device;

struct ImageCreateInfo
{
  std::shared_ptr<const Device>  device;
  VkImage                        handle;
  VkFormat                       format;
  VkColorSpaceKHR                colorSpace;
  VkExtent2D                     extent;
  VkImageUsageFlags              usage;

  explicit ImageCreateInfo(std::shared_ptr<const Device> device,
                           VkImage                       handle,
                           VkFormat                      format,
                           VkColorSpaceKHR               colorSpace,
                           VkExtent2D                    extent,
                           VkImageUsageFlags             usage);
};

class Image : public TObject<VkImage>
{
public:
  explicit Image(const ImageCreateInfo& createInfo);
  ~Image();
protected:
  std::shared_ptr<const Device>  m_device;
  VkFormat                       m_format;
  VkColorSpaceKHR                m_colorSpace;
  VkExtent2D                     m_extent;
  VkImageUsageFlags              m_usage;
};

}
