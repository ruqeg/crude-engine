#include <vulkan/vulkan.hpp>

module crude.graphics.image;

import crude.graphics.device_memory;
import crude.graphics.command_buffer;
import crude.graphics.device;
import crude.graphics.flush;
import crude.graphics.vulkan_utils;

namespace crude::graphics
{

Image::Image(core::shared_ptr<const Device>  device,
             VkImage                         handle,
             VkImageType                     type,
             VkFormat                        format,
             VkExtent3D                      extent)
  :
  m_device(device),
  m_format(format),
  m_usage(0),
  m_extent(extent),
  m_type(type),
  m_layout(VK_IMAGE_LAYOUT_UNDEFINED)
{
  m_handle = handle;
}

Image::Image(core::shared_ptr<const Device>  device,
             VkImageType                     type,
             VkFormat                        format,
             const VkExtent3D&               extent,
             core::uint32                    mipLevels,
             core::uint32                    arrayLayers,
             VkSampleCountFlagBits           samples,
             VkImageCreateFlags              flags,
             VkImageUsageFlags               usage,
             VkImageTiling                   tiling,
             VkSharingMode                   sharingMode)
  :
  m_device(device),
  m_format(format),
  m_usage(usage),
  m_extent(extent),
  m_type(type),
  m_layout(VK_IMAGE_LAYOUT_UNDEFINED)
{
  VkImageCreateInfo vkImageInfo{};
  vkImageInfo.sType                  = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  vkImageInfo.pNext                  = nullptr;
  vkImageInfo.queueFamilyIndexCount  = 0u; // !TODO
  vkImageInfo.pQueueFamilyIndices    = nullptr; // !TODO

  vkImageInfo.flags                  = flags;
  vkImageInfo.imageType              = m_type;
  vkImageInfo.format                 = m_format;
  vkImageInfo.extent                 = m_extent;
  vkImageInfo.mipLevels              = mipLevels;
  vkImageInfo.arrayLayers            = arrayLayers;
  vkImageInfo.samples                = samples;
  vkImageInfo.tiling                 = tiling;
  vkImageInfo.usage                  = m_usage;
  vkImageInfo.sharingMode            = sharingMode;
  vkImageInfo.initialLayout          = m_layout;

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

  core::shared_ptr<Device_Memory> memory = core::allocateShared<Device_Memory>(m_device, getMemoryRequirements(), memoryFlags);
  bindMemory(memory);
}
  
Image::~Image()
{
  if (m_handle != VK_NULL_HANDLE)
  {
    vkDestroyImage(m_device->getHandle(), m_handle, getPVkAllocationCallbacks());
  }
}

void Image::layoutTransition(core::shared_ptr<Command_Buffer> commandBuffer, VkImageLayout newLayout)
{
  VkPipelineStageFlags srcStageMask = 0;
  VkPipelineStageFlags dstStageMask = 0;

  switch (m_layout)
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

  core::shared_ptr<Image> self = core::shared_ptr<Image>(this, [](Image*) {});
  const Image_Memory_Barrier memoryBarrier(self, newLayout, Image_Subresource_Range(self));
  commandBuffer->barrier(srcStageMask, dstStageMask, memoryBarrier);
}

void Image::layoutTransitionUpload(core::shared_ptr<Command_Buffer> commandBuffer, VkImageLayout newLayout)
{
  commandBuffer->begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
  layoutTransition(commandBuffer, newLayout);
  commandBuffer->end();
  flush(commandBuffer);
}

void Image::bindMemory(core::shared_ptr<Device_Memory> memory)
{
  core::shared_ptr<Image> self = core::shared_ptr<Image>(this, [](Image*) {});
  memory->bind(self);
  m_memory = memory;
}

void Image::setLayout(VkImageLayout layout)
{
  m_layout = layout;
}
  
VkImageType Image::getType() const
{
  return m_type;
}

VkFormat Image::getFormat() const
{
  return m_format;
}

VkImageLayout Image::getLayout() const
{
  return m_layout;
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

}
