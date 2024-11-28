module;

#include <vulkan/vulkan.h>

export module crude.gfx.vk.generate_mipmaps;

export import crude.core.std_containers_heap;
import crude.gfx.vk.device;
import crude.gfx.vk.command_buffer;
import crude.gfx.vk.image;
import crude.core.assert;
import crude.gfx.vk.image_blit_region;

export namespace crude::gfx::vk
{

bool generateMipmaps(core::shared_ptr<Command_Buffer> commandBuffer, core::shared_ptr<Image> image, VkFilter filter, VkImageLayout layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VkPipelineStageFlags stage = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT)
{
  core::assert(image.get());
  core::assert(commandBuffer.get());

  if (!image || !commandBuffer) return false;

  if (image->getMipLevelsCount() <= 1) return true;

  // !TODO

  core::int32 prevMipWidth = image->getWidth();
  core::int32 prevMipHeight = image->getHeight();

  const Image_Subresource_Range firstMipRange(image, 0u, 1u, 0u, image->getArrayLayersCount());
  commandBuffer->barrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
    Image_Memory_Barrier(image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, firstMipRange));

  for (core::uint32 mipLevel = 1; mipLevel < image->getMipLevelsCount(); mipLevel++)
  {
    core::int32 nextMipWidth = prevMipWidth > 1 ? prevMipWidth / 2 : 1;
    core::int32 nextMipHeight = prevMipHeight > 1 ? prevMipHeight / 2 : 1;

    Image_Blit_Region blitRegion({
      .srcImage = image,
      .dstImage = image,
      .srcLayers = Image_Subresource_Layers(image, mipLevel - 1, 0, 1),
      .dstLayers = Image_Subresource_Layers(image, mipLevel, 0, 1),
      .srcOffsets = core::array<VkOffset3D, 2>
      {
        VkOffset3D{ 0, 0, 0 },
        VkOffset3D{ prevMipWidth, prevMipHeight, 1 }
      },
      .dstOffsets = core::array<VkOffset3D, 2>
      {
        VkOffset3D{ 0, 0, 0 },
        VkOffset3D{ nextMipWidth, nextMipHeight, 1 }
      } });

    const Image_Subresource_Range nextMipRange(image, mipLevel, 1, 0u, image->getArrayLayersCount());
    commandBuffer->barrier(VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
      Image_Memory_Barrier(image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, nextMipRange));

    commandBuffer->blitImage(blitRegion, filter);

    commandBuffer->barrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
      Image_Memory_Barrier(image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, nextMipRange));

    prevMipWidth = nextMipWidth;
    prevMipHeight = nextMipHeight;
  }

  const Image_Subresource_Range allRange(image);
  commandBuffer->barrier(VK_PIPELINE_STAGE_TRANSFER_BIT, stage, Image_Memory_Barrier(image, layout, allRange));
  
  return true;
}

}