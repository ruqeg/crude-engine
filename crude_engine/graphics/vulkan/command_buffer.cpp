#include <vulkan/vulkan.hpp>

module crude_engine.graphics.vulkan.command_buffer;

import crude_engine.graphics.vulkan.command_pool;
import crude_engine.graphics.vulkan.device;
import crude_engine.graphics.vulkan.image;
import crude_engine.graphics.vulkan.render_pass;
import crude_engine.graphics.vulkan.framebuffer;
import crude_engine.graphics.vulkan.pipeline;
import crude_engine.graphics.vulkan.pipeline_layout;
import crude_engine.graphics.vulkan.descriptor_set;
import crude_engine.graphics.vulkan.vulkan_utils;
import crude_engine.core.std_containers_heap;
import crude_engine.core.algorithms;
import crude_engine.core.assert;

namespace crude_engine
{

Command_Buffer::Command_Buffer(Shared_Ptr<const Device>  device,
                               Shared_Ptr<Command_Pool>  commandPool,
                               VkCommandBufferLevel      level)
  :
  m_device(device),
  m_commandPool(commandPool)
{
  VkCommandBufferAllocateInfo vkAllocateInfo{};
  vkAllocateInfo.sType               = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  vkAllocateInfo.pNext               = nullptr;
  vkAllocateInfo.commandPool         = m_commandPool->getHandle();
  vkAllocateInfo.level               = level;
  vkAllocateInfo.commandBufferCount  = 1u;


  VkResult result = vkAllocateCommandBuffers(m_device->getHandle(), &vkAllocateInfo, &m_handle);
  vulkanHandleResult(result, "failed to allocate command buffer");
}

bool Command_Buffer::begin(VkCommandBufferUsageFlags flags)
{
  if (m_recording)
    return false;

  VkCommandBufferBeginInfo vkBeginInfo{};
  vkBeginInfo.sType             = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  vkBeginInfo.pNext             = nullptr;
  vkBeginInfo.pInheritanceInfo  = nullptr;
  vkBeginInfo.flags             = flags;

  VkResult result = vkBeginCommandBuffer(m_handle, &vkBeginInfo);
  m_recording = (result == VK_SUCCESS);
  return m_recording;
}

bool Command_Buffer::end()
{
  if (!m_recording)
    return false;

  m_recording = false;

  VkResult result = vkEndCommandBuffer(m_handle);
  return result == VK_SUCCESS;
}

void Command_Buffer::barrier(VkPipelineStageFlags        srcStage, 
                             VkPipelineStageFlags        dstStage, 
                             span<Image_Memory_Barrier>  imageMemoryBarriers)
{
  assert(imageMemoryBarriers.data());

  vector<VkImageMemoryBarrier> pVkImageMemoryBarriers(imageMemoryBarriers.size());
  Algorithms::copy(imageMemoryBarriers.begin(), imageMemoryBarriers.end(), pVkImageMemoryBarriers.begin());

  vkCmdPipelineBarrier(
    m_handle, 
    srcStage, 
    dstStage, 
    0u, 
    0u, 
    nullptr, 
    0u, 
    nullptr, 
    pVkImageMemoryBarriers.size(),
    pVkImageMemoryBarriers.data());

  for (uint32 i = 0u; i < imageMemoryBarriers.size(); ++i)
  {
    imageMemoryBarriers[i].m_image->setLayout(imageMemoryBarriers[i].newLayout);
  }
}
  
void Command_Buffer::copyBufferToImage(Shared_Ptr<Buffer>       srcBuffer, 
                                       Shared_Ptr<Image>        dstImage, 
                                       span<VkBufferImageCopy>  regions)
{
  assert(srcBuffer);
  assert(dstImage);
  assert(regions.data());

  vkCmdCopyBufferToImage(
    m_handle, 
    srcBuffer->getHandle(),
    dstImage->getHandle(),
    dstImage->getLayout(), 
    regions.size(),
    regions.data());
}

bool Command_Buffer::reset(VkCommandBufferResetFlags flags)
{
  const VkResult result = vkResetCommandBuffer(m_handle, flags);
  return result != VK_ERROR_OUT_OF_DEVICE_MEMORY;
}

void Command_Buffer::beginRenderPass(Shared_Ptr<Render_Pass>  renderPass,
                                     Shared_Ptr<Framebuffer>  framebuffer,
                                     span<VkClearValue>       clearValues,
                                     const VkRect2D&          renderArea, 
                                     VkSubpassContents        contents)
{
  assert(renderPass);
  assert(framebuffer);
  assert(clearValues.data());

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType            = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.pNext            = nullptr;
  renderPassInfo.renderPass       = renderPass->getHandle();
  renderPassInfo.framebuffer      = framebuffer->getHandle();
  renderPassInfo.renderArea       = renderArea;
  renderPassInfo.clearValueCount  = clearValues.size();
  renderPassInfo.pClearValues     = clearValues.data();
  
  vkCmdBeginRenderPass(
    m_handle,
    &renderPassInfo,
    contents);

  m_withinRenderPass = true;
}

void Command_Buffer::bindPipeline(Shared_Ptr<Pipeline> pipeline)
{
  assert(pipeline);

  vkCmdBindPipeline(
    m_handle,
    pipeline->getBindPoint(),
    pipeline->getHandle());
}

void Command_Buffer::setViewport(span<VkViewport> viewports)
{
  constexpr uint32 offset = 0u;
  vkCmdSetViewport(
    m_handle,
    offset,
    viewports.size(),
    viewports.data());
}

void Command_Buffer::setScissor(span<VkRect2D> scissors)
{
  constexpr uint32 offset = 0u;
  vkCmdSetScissor(
    m_handle,
    offset,
    scissors.size(),
    scissors.data());
}

void Command_Buffer::bindDescriptorSets(Shared_Ptr<Pipeline>              pipeline,
                                        span<Shared_Ptr<Descriptor_Set>>  descriptorSets, 
                                        span<uint32>                      dynamicOffsets)
{
  constexpr uint32 offset = 0u;

  vector<VkDescriptorSet> descriptorSetsHandles(descriptorSets.size());
  Algorithms::copyc(descriptorSets.begin(), descriptorSets.end(), descriptorSetsHandles.begin(), [](auto& src, auto& dst) -> void {
    *dst = (*src)->getHandle();
  });

  vkCmdBindDescriptorSets(
    m_handle,
    pipeline->getBindPoint(),
    pipeline->getPipelineLayout()->getHandle(),
    offset,
    descriptorSetsHandles.size(),
    descriptorSetsHandles.data(),
    dynamicOffsets.size(),
    dynamicOffsets.data());
}

void Command_Buffer::draw(uint32 vertexCount, uint32 instanceCount, uint32 firstVertex, uint32 firstInstance)
{
  vkCmdDraw(m_handle, vertexCount, instanceCount, firstVertex, firstInstance);
}

void Command_Buffer::endRenderPass()
{
  if (m_withinRenderPass)
  {
    vkCmdEndRenderPass(m_handle);
    m_withinRenderPass = false;
  }
}

Command_Buffer::~Command_Buffer()
{
  vkFreeCommandBuffers(
    m_device->getHandle(),
    m_commandPool->getHandle(),
    1u, 
    &m_handle);
}

}