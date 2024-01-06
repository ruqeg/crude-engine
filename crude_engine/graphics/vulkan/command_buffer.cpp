#include <graphics/vulkan/command_buffer.hpp>
#include <graphics/vulkan/command_pool.hpp>
#include <graphics/vulkan/device.hpp>
#include <graphics/vulkan/image.hpp>
#include <graphics/vulkan/buffer.hpp>
#include <graphics/vulkan/render_pass.hpp>
#include <graphics/vulkan/framebuffer.hpp>
#include <graphics/vulkan/pipeline.hpp>
#include <graphics/vulkan/pipeline_layout.hpp>
#include <graphics/vulkan/descriptor_set.hpp>

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
  vkAllocateInfo.commandPool         = CRUDE_OBJECT_HANDLE(m_commandPool);
  vkAllocateInfo.level               = level;
  vkAllocateInfo.commandBufferCount  = 1u;


  VkResult result = vkAllocateCommandBuffers(CRUDE_OBJECT_HANDLE(m_device), &vkAllocateInfo, &m_handle);
  CRUDE_VULKAN_HANDLE_RESULT(result, "failed to allocate command buffer");
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

void Command_Buffer::barrier(VkPipelineStageFlags  srcStage,
                             VkPipelineStageFlags  dstStage, 
                             Image_Memory_Barrier* pImageMemoryBarriers, 
                             uint32                imageMemoryBarrierCount)
{
  CRUDE_ASSERT(pImageMemoryBarriers);

  auto pVkImageMemoryBarriers = Memory_System::Default_Allocator::mnewArray<VkImageMemoryBarrier>(imageMemoryBarrierCount);
  Algorithms::copy(pImageMemoryBarriers, pImageMemoryBarriers + imageMemoryBarrierCount, pVkImageMemoryBarriers);

  vkCmdPipelineBarrier(
    m_handle, 
    srcStage, 
    dstStage, 
    0u, 
    0u, 
    nullptr, 
    0u, 
    nullptr, 
    imageMemoryBarrierCount,
    pVkImageMemoryBarriers);

  for (uint32 i = 0u; i < imageMemoryBarrierCount; ++i)
  {
    pImageMemoryBarriers[i].m_image->setLayout(pImageMemoryBarriers[i].newLayout);
  }
}
  
void Command_Buffer::copyBufferToImage(Buffer*            pSrcBuffer,
                                       Image*             pDstImage, 
                                       VkBufferImageCopy* pRegions, 
                                       uint32             regionCount)
{
  CRUDE_ASSERT(pSrcBuffer);
  CRUDE_ASSERT(pDstImage);
  CRUDE_ASSERT(pRegions);

  vkCmdCopyBufferToImage(
    m_handle, 
    CRUDE_OBJECT_HANDLE(pSrcBuffer), 
    CRUDE_OBJECT_HANDLE(pDstImage), 
    pDstImage->getLayout(), 
    regionCount,
    pRegions);
}

bool Command_Buffer::reset(VkCommandBufferResetFlags flags)
{
  const VkResult result = vkResetCommandBuffer(m_handle, flags);
  return result != VK_ERROR_OUT_OF_DEVICE_MEMORY;
}

void Command_Buffer::beginRenderPass(Render_Pass*       pRenderPass,
                                     Framebuffer*       pFramebuffer,
                                     VkClearValue*      pClearValues,
                                     uint32             clearValueCount,
                                     const VkRect2D&    renderArea, 
                                     VkSubpassContents  contents)
{
  CRUDE_ASSERT(pRenderPass);
  CRUDE_ASSERT(pFramebuffer);
  CRUDE_ASSERT(pClearValues);

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType            = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.pNext            = nullptr;
  renderPassInfo.renderPass       = CRUDE_OBJECT_HANDLE(pRenderPass);
  renderPassInfo.framebuffer      = CRUDE_OBJECT_HANDLE(pFramebuffer);
  renderPassInfo.renderArea       = renderArea;
  renderPassInfo.clearValueCount  = clearValueCount;
  renderPassInfo.pClearValues     = pClearValues;
  
  vkCmdBeginRenderPass(
    m_handle,
    &renderPassInfo,
    contents);

  m_withinRenderPass = true;
}

void Command_Buffer::bindPipeline(Pipeline* pPipeline)
{
  CRUDE_ASSERT(pPipeline);

  vkCmdBindPipeline(
    m_handle,
    pPipeline->getBindPoint(),
    CRUDE_OBJECT_HANDLE(pPipeline));
}

void Command_Buffer::setViewport(VkViewport* pViewports, uint32 viewportCount)
{
  CRUDE_ASSERT(pViewports);

  constexpr uint32 offset = 0u;
  vkCmdSetViewport(
    m_handle,
    offset,
    viewportCount,
    pViewports);
}

void Command_Buffer::setScissor(VkRect2D* pScissors, uint32 scissorCount)
{
  CRUDE_ASSERT(pScissors);

  constexpr uint32 offset = 0u;
  vkCmdSetScissor(
    m_handle,
    offset,
    scissorCount,
    pScissors);
}

void Command_Buffer::bindDescriptorSets(Pipeline*         pPipeline,
                                        Descriptor_Set**  pDescriptorSets, 
                                        uint32            descriptorSetCount, 
                                        uint32*           pDynamicOffsets,
                                        uint32            dynamicOffsetCount)
{
  CRUDE_ASSERT(pPipeline);
  CRUDE_ASSERT(pDescriptorSets);
  CRUDE_ASSERT(pDynamicOffsets);

  constexpr uint32 offset = 0u;

  auto pDescriptorSetsHandles = Memory_System::Default_Allocator::mnewArray<VkDescriptorSet>(descriptorSetCount);
  Algorithms::copyc(pDescriptorSets, pDescriptorSets + descriptorSetCount, pDescriptorSetsHandles, [](Descriptor_Set* s, VkDescriptorSet* d) -> void {
    *d = CRUDE_OBJECT_HANDLE(s);
  });

  vkCmdBindDescriptorSets(
    m_handle,
    pPipeline->getBindPoint(),
    CRUDE_OBJECT_HANDLE(pPipeline->getPipelineLayout()),
    offset,
    descriptorSetCount,
    pDescriptorSetsHandles,
    dynamicOffsetCount,
    pDynamicOffsets);
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
    CRUDE_OBJECT_HANDLE(m_device), 
    CRUDE_OBJECT_HANDLE(m_commandPool), 
    1u, 
    &m_handle);
}

}