module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.image_view;

export import crude.core.std_containers_heap;
export import crude.graphics.image_subresource_range;
import crude.graphics.vulkan_object;

export namespace crude::graphics
{

class Device;
class Image;

class Image_View : public Vulkan_Object<VkImageView>
{
public:
  explicit Image_View(core::shared_ptr<Image> image);
  explicit Image_View(core::shared_ptr<Image>         image,
                      VkFormat                        format,
                      const Image_Subresource_Range&  subresourceRange,
                      const VkComponentMapping&       components = {});
  explicit Image_View(core::shared_ptr<Image>         image,
                      const Image_Subresource_Range&  subresourceRange,
                      const VkComponentMapping&       components = {});
  ~Image_View();
  core::shared_ptr<const Image> getImage() const;
  core::shared_ptr<Image> getImage();
  const Image_Subresource_Range& getSubresourceRange() const { return m_subresourceRange; }
private:
  VkImageViewType imageToViewType(VkImageType imageType, VkImageCreateFlags imageFlags, core::uint32 arrayLayersCount);
private:
  core::shared_ptr<Image>         m_image;
  const Image_Subresource_Range   m_subresourceRange;
};

}
