module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.image_cube;

export import crude.gfx.vk.image;

export namespace crude::gfx::vk
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
                      core::uint32                    arrayCount,
                      VkSampleCountFlagBits           samples,
                      VkImageCreateFlags              flags,
                      VkImageUsageFlags               usage,
                      VkImageTiling                   tiling,
                      VkSharingMode                   sharingMode);
};

}