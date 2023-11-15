#pragma once

#include "core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"

namespace crude_vulkan_01 
{

class Device;

struct Image_From_Handle_Create_Info
{
  std::shared_ptr<const Device>  device;
  VkImage                        handle;
  VkFormat                       format;
  VkExtent3D                     extent;
  VkImageUsageFlags              usage;
  VkImageType                    type;

  explicit Image_From_Handle_Create_Info(std::shared_ptr<const Device>  device,
                                         VkImage                        handle,
                                         VkFormat                       format,
                                         VkExtent3D                     extent,
                                         VkImageUsageFlags              usage,
                                         VkImageType                    type);
};

struct Image_2D_Create_Info
{
  std::shared_ptr<const Device>  device;
  VkImageCreateFlags             flags;
  VkFormat                       format;
  VkExtent2D                     extent;
  uint32                         mipLevels;
  uint32                         arrayLayers;
  VkSampleCountFlagBits          samples;
  VkImageTiling                  tiling;
  VkImageUsageFlags              usage;
  VkSharingMode                  sharingMode;
  VkImageLayout                  initialLayout;

  explicit Image_2D_Create_Info(std::shared_ptr<const Device>  device,
                                VkImageCreateFlags             flags,
                                VkFormat                       format,
                                const VkExtent2D&              extent,
                                uint32                         mipLevels,
                                uint32                         arrayLayers,
                                VkSampleCountFlagBits          samples,
                                VkImageTiling                  tiling,
                                VkImageUsageFlags              usage,
                                VkSharingMode                  sharingMode,
                                VkImageLayout                  initialLayout);
};

class Image : public TObject<VkImage>
{
public:
  explicit Image(const Image_From_Handle_Create_Info& createInfo);
  explicit Image(const Image_2D_Create_Info& createInfo);
  ~Image();
  void setLayout(VkImageLayout layout);
  VkImageType getType() const;
  VkFormat getFormat() const;
  VkImageLayout getLayout() const;
  VkMemoryRequirements getMemoryRequirements() const;
protected:
  std::shared_ptr<const Device>  m_device;
  VkFormat                       m_format;
  VkExtent3D                     m_extent;
  VkImageUsageFlags              m_usage;
  VkImageType                    m_type;
  VkImageLayout                  m_layout;
};

}
