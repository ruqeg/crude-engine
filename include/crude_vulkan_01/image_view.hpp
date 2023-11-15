#pragma once

#include "core.hpp"
#include "object.hpp"
#include "include_vulkan.hpp"
#include "image_subresource_range.hpp"

namespace crude_vulkan_01 
{

class Device;
class Image;

struct Image_View_Create_Info
{
  std::shared_ptr<const Device>  device;
  std::shared_ptr<const Image>   image;
  VkFormat                       format;
  Image_Subresource_Range        subresourceRange;
  VkComponentMapping             components;

  explicit Image_View_Create_Info(std::shared_ptr<const Device>   device,
                                  std::shared_ptr<const Image>    image,
                                  VkFormat                        format,
                                  const Image_Subresource_Range&  subresourceRange,
                                  const VkComponentMapping&       components = {});
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
