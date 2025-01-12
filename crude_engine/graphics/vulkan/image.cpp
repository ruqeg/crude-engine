#include <vulkan/vulkan.hpp>

module crude.gfx.vk.image;

import crude.gfx.vk.device_memory;
import crude.gfx.vk.staging_buffer;
import crude.gfx.vk.command_buffer;
import crude.gfx.vk.device;
import crude.gfx.vk.flush;
import crude.gfx.vk.vulkan_utils;

namespace crude::gfx::vk
{

Image::Image(core::shared_ptr<const Device>  device,
             VkImage                         handle,
             VkImageType                     type,
             VkFormat                        format,
             VkExtent3D                      extent)
  : m_device{ device }
  , m_format{ format }
  , m_usage{ 0 }
  , m_extent{ extent }
  , m_type{ type }
  , m_layouts{ 1u, VK_IMAGE_LAYOUT_UNDEFINED }
  , m_mipLevelsCount{ 1u }
  , m_arrayLayersCount{ 1u }
  , m_samples{ VK_SAMPLE_COUNT_1_BIT }
  , m_flags{}
{
  m_handle = handle;
}

Image::Image(core::shared_ptr<const Device>  device,
             VkImageType                     type,
             VkFormat                        format,
             const VkExtent3D&               extent,
             core::uint32                    mipLevelsCount,
             core::uint32                    arrayLayersCount,
             VkSampleCountFlagBits           samples,
             VkImageCreateFlags              flags,
             VkImageUsageFlags               usage,
             VkImageTiling                   tiling,
             VkSharingMode                   sharingMode)
  : m_device{ device }
  , m_format{ format }
  , m_usage{ usage }
  , m_extent{ extent }
  , m_type{ type }
  , m_layouts{ mipLevelsCount, VK_IMAGE_LAYOUT_UNDEFINED }
  , m_mipLevelsCount{ mipLevelsCount }
  , m_arrayLayersCount{ arrayLayersCount }
  , m_samples{ samples }
  , m_flags{ flags }
{
  VkImageCreateInfo vkImageInfo{};
  vkImageInfo.sType                  = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  vkImageInfo.pNext                  = nullptr;
  vkImageInfo.queueFamilyIndexCount  = 0u; // !TODO
  vkImageInfo.pQueueFamilyIndices    = nullptr; // !TODO

  vkImageInfo.flags                  = m_flags;
  vkImageInfo.imageType              = m_type;
  vkImageInfo.format                 = m_format;
  vkImageInfo.extent                 = m_extent;
  vkImageInfo.mipLevels              = mipLevelsCount;
  vkImageInfo.arrayLayers            = arrayLayersCount;
  vkImageInfo.samples                = samples;
  vkImageInfo.tiling                 = tiling;
  vkImageInfo.usage                  = m_usage;
  vkImageInfo.sharingMode            = sharingMode;
  vkImageInfo.initialLayout          = m_layouts.front();

  VkResult result = vkCreateImage(m_device->getHandle(), &vkImageInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "Failed to create image");

  VkMemoryPropertyFlags memoryFlags;
  if (tiling == VK_IMAGE_TILING_LINEAR)
  {
    memoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
  }
  else
  {
    memoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
  }

  m_memory = core::allocateShared<Device_Memory>(m_device, getMemoryRequirements(), memoryFlags);
  m_memory->bind(core::makeUnsafeSharedWithEmptyDestructor(this));
}
  
Image::~Image()
{
  if (m_handle != VK_NULL_HANDLE)
  {
    vkDestroyImage(m_device->getHandle(), m_handle, getPVkAllocationCallbacks());
  }
}

void Image::fillLayout(VkImageLayout newLayout)
{
  std::fill(m_layouts.begin(), m_layouts.end(), newLayout);
}

void Image::transitionMipLayout(core::shared_ptr<Command_Buffer> commandBuffer, core::uint32 mipLevel, VkImageLayout newLayout)
{
  VkPipelineStageFlags srcStageMask = 0;
  VkPipelineStageFlags dstStageMask = 0;

  switch (getMipLayout(mipLevel))
  {
  case VK_IMAGE_LAYOUT_UNDEFINED:
    srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    break;
  default:
    return;
  }

  switch (newLayout)
  {
  case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
    dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    break;
  default:
    return;
  }

  const std::shared_ptr<Image> self = core::makeUnsafeSharedWithEmptyDestructor(this);
  const Image_Memory_Barrier memoryBarrier(self, newLayout, Image_Subresource_Range(self, mipLevel, 1u, 0u, m_arrayLayersCount));
  commandBuffer->barrier(srcStageMask, dstStageMask, memoryBarrier);
}

void Image::transitionMipLayoutUpload(core::shared_ptr<Command_Buffer> commandBuffer, core::uint32 mipLevel, VkImageLayout newLayout)
{
  commandBuffer->begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
  transitionMipLayout(commandBuffer, mipLevel, newLayout);
  commandBuffer->end();
  flush(commandBuffer);
}

void Image::bindMemory(core::shared_ptr<Device_Memory> memory)
{
  memory->bind(shared_from_this());
  m_memory = memory;
}

void Image::stagedUpload(core::shared_ptr<Command_Buffer>  commandBuffer,
                            const Mip_Data&                mipMap,
                            core::uint32                   mipLevel,
                            core::uint32                   arrayLayer,
                            VkImageLayout                  dstLayout,
                            VkPipelineStageFlags           dstStageMask)
{
  core::shared_ptr<Staging_Buffer> stagingBuffer = core::allocateShared<Staging_Buffer>(m_device, mipMap.texels);
  commandBuffer->begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
  copyMipFromBuffer(commandBuffer, stagingBuffer, {}, mipLevel, arrayLayer, dstLayout, dstStageMask);
  commandBuffer->end();
  flush(commandBuffer);
}

void Image::setMipLayout(core::uint32 mipLevel, VkImageLayout layout)
{
  // !TODO
  m_layouts[mipLevel] = layout;
}

VkImageLayout Image::getMipLayout(core::uint32 mipLevel) const
{
  // !TODO
  return m_layouts[mipLevel];
}

VkImageLayout Image::getLayout() const
{
  // !TODO
  return m_layouts[0];
}

void Image::copyMipFromBuffer(core::shared_ptr<Command_Buffer>        commandBuffer,
                              core::shared_ptr<const Staging_Buffer>  srcBuffer,
                              const Copy_Layout&                      bufferLayout,
                              core::uint32                            mipLevel,
                              core::uint32                            arrayLayer,
                              VkImageLayout                           dstLayout,
                              VkPipelineStageFlags                    dstStageMask)
{
  core::shared_ptr<Image> self = core::makeUnsafeSharedWithEmptyDestructor(this);
  const Image_Subresource_Range imageRange(self);

  VkBufferImageCopy region;
  region.bufferOffset                     = bufferLayout.offset;
  region.bufferRowLength                  = bufferLayout.rowLength;
  region.bufferImageHeight                = bufferLayout.imageHeight;

  region.imageSubresource.aspectMask      = imageRange.aspectMask;
  region.imageSubresource.mipLevel        = mipLevel;
  region.imageSubresource.baseArrayLayer  = 0u;
  region.imageSubresource.layerCount      = arrayLayer;

  region.imageOffset                      = {0, 0, 0};
  region.imageExtent                      = m_extent;


  const Image_Memory_Barrier transferDst(self, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, imageRange);
  commandBuffer->barrier(VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, transferDst);
  commandBuffer->copyBufferToImage(srcBuffer, self, region);
  const Image_Memory_Barrier shaderRead(self, dstLayout, imageRange);
  commandBuffer->barrier(VK_PIPELINE_STAGE_TRANSFER_BIT, dstStageMask, shaderRead);
}
  
VkImageType Image::getType() const
{
  return m_type;
}

VkFormat Image::getFormat() const
{
  return m_format;
}

VkMemoryRequirements Image::getMemoryRequirements() const
{
  VkMemoryRequirements memRequirements;
  vkGetImageMemoryRequirements(m_device->getHandle(), m_handle, &memRequirements);
  return memRequirements;
}

core::shared_ptr<const Device> Image::getDevice() const
{
  return m_device;
}

VkExtent3D Image::calculateMipExtent(core::uint32 mipLevel) const
{
  // !TODO
  VkExtent3D mipExtent = { calculateMipWidth(mipLevel), calculateMipHeight(mipLevel), m_extent.depth};
  return mipExtent;
}

core::uint32 Image::calculateMipWidth(core::uint32 mipLevel) const
{
  // !TODO
  return m_extent.width > 1 ? m_extent.width / static_cast<core::float32>(1 << mipLevel) : 1;
}

core::uint32 Image::calculateMipHeight(core::uint32 mipLevel) const
{
  // !TODO
  return m_extent.height > 1 ? m_extent.height / static_cast<core::float32>(1 << mipLevel) : 1;
}

}
