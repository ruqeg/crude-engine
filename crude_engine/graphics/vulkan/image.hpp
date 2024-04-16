#pragma once

#include <core/shared_ptr.hpp>
#include <graphics/vulkan/object.hpp>
#include <graphics/vulkan/include_vulkan.hpp>

namespace crude_engine 
{

class Device;

class Image : public TObject<VkImage>
{
public:
  // create image from handle
  explicit Image(Shared_Ptr<const Device>  device,
                 VkImage                   handle,
                 VkFormat                  format,
                 VkExtent3D                extent,
                 VkImageUsageFlags         usage,
                 VkImageType               type);

  // create image 2d
  explicit Image(Shared_Ptr<const Device>  device,
                 VkImageCreateFlags        flags,
                 VkFormat                  format,
                 const VkExtent2D&         extent,
                 uint32                    mipLevels,
                 uint32                    arrayLayers,
                 VkSampleCountFlagBits     samples,
                 VkImageTiling             tiling,
                 VkImageUsageFlags         usage,
                 VkSharingMode             sharingMode,
                 VkImageLayout             initialLayout);
  ~Image();
  void setLayout(VkImageLayout layout);
  VkImageType getType() const;
  VkFormat getFormat() const;
  VkImageLayout getLayout() const;
  VkMemoryRequirements getMemoryRequirements() const;
protected:
  const Shared_Ptr<const Device>  m_device;
  VkFormat                        m_format;
  VkExtent3D                      m_extent;
  VkImageUsageFlags               m_usage;
  VkImageType                     m_type;
  VkImageLayout                   m_layout;
};

}
