#pragma once

#include "core.hpp"
#include "object.hpp"
#include "include_vulkan.hpp"
#include <memory>

namespace crude_vulkan_01 
{

class Device;
class Image;

struct Image_View_Create_Info
{
  std::shared_ptr<const Device>  device;
  std::shared_ptr<const Image>   image;
  VkFormat                       format;
  VkComponentMapping             components;
  VkImageAspectFlags             aspectMask;
  uint32                         baseArrayLayer;
  uint32                         baseMipLevel;
  uint32                         levelCount;
  uint32                         layerCount;
  explicit Image_View_Create_Info(std::shared_ptr<const Device>  device,
                                  std::shared_ptr<const Image>   image,
                                  VkFormat                       format,
                                  VkComponentMapping             components,
                                  VkImageAspectFlags             aspectMask,
                                  uint32                         baseArrayLayer,
                                  uint32                         baseMipLevel,
                                  uint32                         levelCount,
                                  uint32                         layerCount);
};

class Image_View : public TObject<VkImageView>
{
public:
  explicit Image_View(const Image_View_Create_Info& createInfo);
  ~Image_View();
private:
  VkImageViewType imageToViewType(VkImageType imageType);
private:
  std::shared_ptr<const Device>  m_device;
  std::shared_ptr<const Image>   m_image;
};

}
