module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.image_subresource_layers;

export import crude.core.std_containers_heap;

export namespace crude::graphics
{

class Image;

class Image_Subresource_Layers : public VkImageSubresourceLayers
{
public:
  explicit Image_Subresource_Layers(core::shared_ptr<const Image>  image);
  explicit Image_Subresource_Layers(core::shared_ptr<const Image>  image,
                                    core::uint32                   mipLevel,
                                    core::uint32                   baseArrayLayer,
                                    core::uint32                   layerCount);
};

}