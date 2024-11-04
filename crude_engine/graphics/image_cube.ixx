module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.image_cube;

export import crude.graphics.image;

export namespace crude::graphics
{

class Image_Cube : public Image
{
public:
  explicit Image_Cube(core::shared_ptr<const Device>  device,
                     VkFormat                         format,
                     core::uint32                     dimension,
                     core::uint32                     mipLevelsCount,
                     VkSharingMode                    sharingMode);
protected:
  explicit Image_Cube(core::shared_ptr<const Device>  device,
                      VkFormat                        format,
                      core::uint32                    dimension,
                      core::uint32                    mipLevelsCount,
                      VkSampleCountFlagBits           samples,
                      VkImageCreateFlags              flags,
                      VkImageUsageFlags               usage,
                      VkImageTiling                   tiling,
                      VkSharingMode                   sharingMode);
};

}