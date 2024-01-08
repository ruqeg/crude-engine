#pragma once

#include <core/data_structures/shared_ptr.hpp>
#include <graphics/vulkan/object.hpp>
#include <graphics/vulkan/image_subresource_range.hpp>

namespace crude_engine 
{

class Device;
class Image;

class Image_View : public TObject<VkImageView>
{
public:
  explicit Image_View(Shared_Ptr<const Device>        device,
                      Shared_Ptr<const Image>         image,
                      VkFormat                        format,
                      const Image_Subresource_Range&  subresourceRange,
                      const VkComponentMapping&       components = {});
  ~Image_View();
private:
  VkImageViewType imageToViewType(VkImageType imageType);
private:
  Shared_Ptr<const Device>  m_device;
  Shared_Ptr<const Image>   m_image;
};

}
