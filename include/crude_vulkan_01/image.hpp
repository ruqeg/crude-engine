#pragma once

#include "core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"
#include <vulkan/vulkan_core.h>

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
  VkImageType                    type;

  explicit ImageCreateInfo(std::shared_ptr<const Device> device,
                           VkImage                       handle,
                           VkFormat                      format,
                           VkColorSpaceKHR               colorSpace,
                           VkExtent2D                    extent,
                           VkImageUsageFlags             usage,
                           VkImageType                   type);
};

class Image : public TObject<VkImage>
{
public:
  explicit Image(const ImageCreateInfo& createInfo);
  ~Image();
  VkImageType getType() const;
protected:
  std::shared_ptr<const Device>  m_device;
  VkFormat                       m_format;
  VkColorSpaceKHR                m_colorSpace;
  VkExtent2D                     m_extent;
  VkImageUsageFlags              m_usage;
  VkImageType                    m_type;
};

}
