module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.image_blit_region;

export import crude.graphics.image_subresource_layers;
export import crude.core.std_containers_stack;

export namespace crude::graphics
{

class Image;

class Image_Blit_Region : public VkImageBlit
{
public:
  struct Initialize;
public:
  explicit Image_Blit_Region(core::shared_ptr<Image> srcImage, core::shared_ptr<Image> dstImage);
  explicit Image_Blit_Region(const Initialize& initialize);
  core::shared_ptr<Image> getSourceImage() const;
  core::shared_ptr<Image> getDestinationImage() const;
private:
  core::shared_ptr<Image>  m_srcImage;
  core::shared_ptr<Image>  m_dstImage;
};

struct Image_Blit_Region::Initialize
{
  core::shared_ptr<Image>     srcImage;
  core::shared_ptr<Image>     dstImage;
  Image_Subresource_Layers    srcLayers;
  Image_Subresource_Layers    dstLayers;
  core::array<VkOffset3D, 2>  srcOffsets;
  core::array<VkOffset3D, 2>  dstOffsets;
};

}