module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.image;

import crude.core.std_containers_heap;
import crude.graphics.vulkan_object;

export namespace crude::graphics
{

class Device;

class Image : public Vulkan_Object<VkImage>
{
public:
  // create image from handle
  explicit Image(core::shared_ptr<const Device>  device,
                 VkImage                         handle,
                 VkFormat                        format,
                 VkExtent3D                      extent,
                 VkImageUsageFlags               usage,
                 VkImageType                     type);

  // create image 2d
  explicit Image(core::shared_ptr<const Device>  device,
                 VkImageCreateFlags              flags,
                 VkFormat                        format,
                 const VkExtent2D&               extent,
                 core::uint32                    mipLevels,
                 core::uint32                    arrayLayers,
                 VkSampleCountFlagBits           samples,
                 VkImageTiling                   tiling,
                 VkImageUsageFlags               usage,
                 VkSharingMode                   sharingMode,
                 VkImageLayout                   initialLayout);
  ~Image();
  void setLayout(VkImageLayout layout);
  VkImageType getType() const;
  VkFormat getFormat() const;
  VkImageLayout getLayout() const;
  VkMemoryRequirements getMemoryRequirements() const;
protected:
  const core::shared_ptr<const Device>  m_device;
  VkFormat                              m_format;
  VkExtent3D                            m_extent;
  VkImageUsageFlags                     m_usage;
  VkImageType                           m_type;
  VkImageLayout                         m_layout;
};

}
