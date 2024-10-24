module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.image_attachment;

export import crude.graphics.image_2d;

export namespace crude::graphics
{

class Depth_Stencil_Attachment : public Image_2D
{
public:
  struct Initialize;
public:
  explicit Depth_Stencil_Attachment(const Initialize& initialize);
};

class Color_Attachment : public Image_2D
{
public:
  struct Initialize;
public:
  explicit Color_Attachment(const Initialize& initialize);
};

struct Color_Attachment::Initialize
{
  core::shared_ptr<const Device>  device;
  VkFormat                        format;
  VkExtent2D                      extent;
  bool                            sampled;
  bool                            explicitResolve;
  core::uint32                    mipLevelsCount;
  VkSampleCountFlagBits           samples;
};

struct Depth_Stencil_Attachment::Initialize
{
  core::shared_ptr<const Device>  device;
  VkFormat                        depthStencilFormat;
  VkExtent2D                      extent;
  bool                            sampled;
  bool                            explicitResolve;
  core::uint32                    mipLevelsCount;
  VkSampleCountFlagBits           samples;
};

Depth_Stencil_Attachment::Depth_Stencil_Attachment(const Initialize& initialize)
  :
  Image_2D(initialize.device, initialize.depthStencilFormat, initialize.extent, initialize.mipLevelsCount,
    1u, initialize.samples, 0u,
    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT 
      | (initialize.sampled ? VK_IMAGE_USAGE_SAMPLED_BIT : 0)
      | (initialize.explicitResolve ? (initialize.samples != VK_SAMPLE_COUNT_1_BIT ? VK_IMAGE_USAGE_TRANSFER_SRC_BIT : VK_IMAGE_USAGE_TRANSFER_DST_BIT) : 0),
    VK_IMAGE_TILING_OPTIMAL, 
    VK_SHARING_MODE_EXCLUSIVE)
{}

Color_Attachment::Color_Attachment(const Initialize& initialize)
  :
  Image_2D(initialize.device, initialize.format, initialize.extent, initialize.mipLevelsCount,
    1u, initialize.samples, 0u,
    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
      | (initialize.sampled ? VK_IMAGE_USAGE_SAMPLED_BIT : 0)
      | (initialize.explicitResolve ? (initialize.samples != VK_SAMPLE_COUNT_1_BIT ? VK_IMAGE_USAGE_TRANSFER_SRC_BIT : VK_IMAGE_USAGE_TRANSFER_DST_BIT) : 0),
    VK_IMAGE_TILING_OPTIMAL, VK_SHARING_MODE_EXCLUSIVE)
{}

}