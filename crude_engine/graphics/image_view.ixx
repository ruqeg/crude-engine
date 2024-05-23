module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.image_view;

import crude.core.shared_ptr;
import crude.graphics.vulkan_object;
import crude.graphics.image_subresource_range;

export namespace crude::graphics
{

class Device;
class Image;

class Image_View : public Vulkan_Object<VkImageView>
{
public:
  explicit Image_View(core::Shared_Ptr<const Device>  device,
                      core::Shared_Ptr<const Image>   image,
                      VkFormat                        format,
                      const Image_Subresource_Range&  subresourceRange,
                      const VkComponentMapping&       components = {});
  ~Image_View();
private:
  VkImageViewType imageToViewType(VkImageType imageType);
private:
  core::Shared_Ptr<const Device>  m_device;
  core::Shared_Ptr<const Image>   m_image;
};

}
