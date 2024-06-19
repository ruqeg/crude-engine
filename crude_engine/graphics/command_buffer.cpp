#include <vulkan/vulkan.hpp>
#undef assert

module crude.graphics.command_buffer;

import crude.graphics.command_pool;
import crude.graphics.device;
import crude.graphics.image;
import crude.graphics.render_pass;
import crude.graphics.framebuffer;
import crude.graphics.pipeline;
import crude.graphics.pipeline_layout;
import crude.graphics.buffer;
import crude.graphics.model_buffer;
import crude.graphics.vertex_buffer;
import crude.graphics.index_buffer;
import crude.graphics.descriptor_set;
import crude.graphics.vulkan_utils;
import crude.core.std_containers_heap;
import crude.core.algorithms;
import crude.core.assert;

namespace crude::graphics
{

Command_Buffer::Command_Buffer(core::shared_ptr<Command_Pool>  commandPool,
                               VkCommandBufferLevel            level)
  :
  m_commandPool(commandPool)
{
  VkCommandBufferAllocateInfo vkAllocateInfo{};
  vkAllocateInfo.sType               = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  vkAllocateInfo.pNext               = nullptr;
  vkAllocateInfo.commandPool         = m_commandPool->getHandle();
  vkAllocateInfo.level               = level;
  vkAllocateInfo.commandBufferCount  = 1u;


  VkResult result = vkAllocateCommandBuffers(getDevice()->getHandle(), &vkAllocateInfo, &m_handle);
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

void Command_Buffer::barrier(VkPipelineStageFlags                    srcStage, 
                             VkPipelineStageFlags                    dstStage, 
                             core::span<const Image_Memory_Barrier>  imageMemoryBarriers)
{
  core::assert(imageMemoryBarriers.data());

  core::vector<VkImageMemoryBarrier> pVkImageMemoryBarriers(imageMemoryBarriers.size());
  core::Algorithms::copy(imageMemoryBarriers.begin(), imageMemoryBarriers.end(), pVkImageMemoryBarriers.begin());

  vkCmdPipelineBarrier(
    m_handle, srcStage, dstStage, 0u, 
    0u, nullptr, 
    0u, nullptr, 
    pVkImageMemoryBarriers.size(), pVkImageMemoryBarriers.data());

  for (core::uint32 i = 0u; i < imageMemoryBarriers.size(); ++i)
  {
    imageMemoryBarriers[i].m_image->setLayout(imageMemoryBarriers[i].newLayout);
  }
}

void Command_Buffer::barrier(VkPipelineStageFlags         srcStage, 
                             VkPipelineStageFlags         dstStage, 
                             const Image_Memory_Barrier&  imageMemoryBarrier)
{ 
  vkCmdPipelineBarrier(
    m_handle, srcStage, dstStage, 0u, 
    0u, nullptr, 
    0u,  nullptr, 
    1u, &imageMemoryBarrier);

  imageMemoryBarrier.m_image->setLayout(imageMemoryBarrier.newLayout);
}
  
void Command_Buffer::copyBufferToImage(core::shared_ptr<Buffer>       srcBuffer, 
                                       core::shared_ptr<Image>        dstImage, 
                                       core::span<VkBufferImageCopy>  regions)
{
  core::assert(srcBuffer.get());
  core::assert(dstImage.get());
  core::assert(regions.data());

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

void Command_Buffer::beginRenderPass(core::shared_ptr<Render_Pass>  renderPass,
                                     core::shared_ptr<Framebuffer>  framebuffer,
                                     core::span<VkClearValue>       clearValues,
                                     const VkRect2D&                renderArea, 
                                     VkSubpassContents              contents)
{
  core::assert(renderPass.get());
  core::assert(framebuffer.get());
  core::assert(clearValues.data());

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

void Command_Buffer::bindPipeline(core::shared_ptr<Pipeline> pipeline)
{
  core::assert(pipeline.get());

  vkCmdBindPipeline(
    m_handle,
    pipeline->getBindPoint(),
    pipeline->getHandle());
}

void Command_Buffer::setViewport(core::span<VkViewport> viewports)
{
  constexpr core::uint32 offset = 0u;
  vkCmdSetViewport(
    m_handle,
    offset,
    viewports.size(),
    viewports.data());
}

void Command_Buffer::setScissor(core::span<VkRect2D> scissors)
{
  constexpr core::uint32 offset = 0u;
  vkCmdSetScissor(
    m_handle,
    offset,
    scissors.size(),
    scissors.data());
}

// !TODO should i move this fun?
void Command_Buffer::bindModelBuffer(core::uint32 firstBinding, core::shared_ptr<Model_Buffer> modelBuffer)
{
  bindVertexBuffer(firstBinding, modelBuffer->getVertexBuffer());
  bindIndexBuffer(modelBuffer->getIndexBuffer());
}

void Command_Buffer::bindVertexBuffer(core::uint32 firstBinding, core::shared_ptr<Buffer> vertexBuffer, VkDeviceSize offset)
{
  vkCmdBindVertexBuffers(m_handle, firstBinding, 1u, &vertexBuffer->getHandle(), &offset);
}

void Command_Buffer::bindIndexBuffer(core::shared_ptr<Index_Buffer> indexBuffer, VkDeviceSize offset)
{
  vkCmdBindIndexBuffer(m_handle, indexBuffer->getHandle(), offset, indexBuffer->getIndexType());
}

void Command_Buffer::copyBuffer(core::shared_ptr<const Buffer>  srcBuffer, 
                                core::shared_ptr<Buffer>        dstBuffer, 
                                const VkBufferCopy&             copyRegion)
{
  vkCmdCopyBuffer(m_handle, srcBuffer->getHandle(), dstBuffer->getHandle(), 1, &copyRegion);
}

void Command_Buffer::bindDescriptorSets(core::shared_ptr<Pipeline>                    pipeline,
                                        core::span<core::shared_ptr<Descriptor_Set>>  descriptorSets,
                                        core::span<core::uint32>                      dynamicOffsets)
{
  constexpr core::uint32 offset = 0u;

  core::vector<VkDescriptorSet> descriptorSetsHandles(descriptorSets.size());
  core::Algorithms::copyc(descriptorSets.begin(), descriptorSets.end(), descriptorSetsHandles.begin(), [](auto& src, auto& dst) -> void {
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

void Command_Buffer::draw(core::uint32 vertexCount, core::uint32 instanceCount, core::uint32 firstVertex, core::uint32 firstInstance)
{
  vkCmdDraw(m_handle, vertexCount, instanceCount, firstVertex, firstInstance);
}

void Command_Buffer::drawIndexed(core::uint32 indexCount, core::uint32 instanceCount, core::uint32 firstIndex, core::int32 vertexOffset, core::uint32 firstInstance)
{
  vkCmdDrawIndexed(m_handle, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
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
    getDevice()->getHandle(),
    m_commandPool->getHandle(),
    1u, 
    &m_handle);
}

core::shared_ptr<const Device> Command_Buffer::getDevice() const
{
  return m_commandPool->getDevice();
}

core::shared_ptr<Command_Pool> Command_Buffer::getCommandPool()
{
  return m_commandPool;
}

}