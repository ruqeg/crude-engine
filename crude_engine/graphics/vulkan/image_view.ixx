module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.image_view;

import crude_engine.core.shared_ptr;
import crude_engine.graphics.vulkan.vulkan_object;
import crude_engine.graphics.vulkan.image_subresource_range;

export namespace crude_engine 
{

class Device;
class Image;

class Image_View : public Vulkan_Object<VkImageView>
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
