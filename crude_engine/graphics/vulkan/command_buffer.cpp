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
#include <core/std_containers.hpp>

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

void Command_Buffer::barrier(VkPipelineStageFlags                srcStage, 
                             VkPipelineStageFlags                dstStage, 
                             Array_Unsafe<Image_Memory_Barrier>  imageMemoryBarriers)
{
  CRUDE_ASSERT(imageMemoryBarriers.data());

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
  
void Command_Buffer::copyBufferToImage(Shared_Ptr<Buffer>              srcBuffer, 
                                       Shared_Ptr<Image>               dstImage, 
                                       Array_Unsafe<VkBufferImageCopy> regions)
{
  CRUDE_ASSERT(srcBuffer);
  CRUDE_ASSERT(dstImage);
  CRUDE_ASSERT(regions.data());

  vkCmdCopyBufferToImage(
    m_handle, 
    CRUDE_OBJECT_HANDLE(srcBuffer), 
    CRUDE_OBJECT_HANDLE(dstImage), 
    dstImage->getLayout(), 
    regions.size(),
    regions.data());
}

bool Command_Buffer::reset(VkCommandBufferResetFlags flags)
{
  const VkResult result = vkResetCommandBuffer(m_handle, flags);
  return result != VK_ERROR_OUT_OF_DEVICE_MEMORY;
}

void Command_Buffer::beginRenderPass(Shared_Ptr<Render_Pass>     renderPass,
                                     Shared_Ptr<Framebuffer>     framebuffer,
                                     Array_Unsafe<VkClearValue>  clearValues,
                                     const VkRect2D&             renderArea, 
                                     VkSubpassContents           contents)
{
  CRUDE_ASSERT(renderPass);
  CRUDE_ASSERT(framebuffer);
  CRUDE_ASSERT(clearValues.data());

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType            = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.pNext            = nullptr;
  renderPassInfo.renderPass       = CRUDE_OBJECT_HANDLE(renderPass);
  renderPassInfo.framebuffer      = CRUDE_OBJECT_HANDLE(framebuffer);
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
  CRUDE_ASSERT(pipeline);

  vkCmdBindPipeline(
    m_handle,
    pipeline->getBindPoint(),
    CRUDE_OBJECT_HANDLE(pipeline));
}

void Command_Buffer::setViewport(Array_Unsafe<VkViewport> viewports)
{
  constexpr uint32 offset = 0u;
  vkCmdSetViewport(
    m_handle,
    offset,
    viewports.size(),
    viewports.data());
}

void Command_Buffer::setScissor(Array_Unsafe<VkRect2D> scissors)
{
  constexpr uint32 offset = 0u;
  vkCmdSetScissor(
    m_handle,
    offset,
    scissors.size(),
    scissors.data());
}

void Command_Buffer::bindDescriptorSets(Shared_Ptr<Pipeline>                      pipeline,
                                        Array_Unsafe<Shared_Ptr<Descriptor_Set>>  descriptorSets, 
                                        Array_Unsafe<uint32>                      dynamicOffsets)
{
  constexpr uint32 offset = 0u;

  vector<VkDescriptorSet> descriptorSetsHandles(descriptorSets.size());
  Algorithms::copyc(descriptorSets.begin(), descriptorSets.end(), descriptorSetsHandles.begin(), [](auto& src, auto& dst) -> void {
    *dst = CRUDE_OBJECT_HANDLE(*src);
  });

  vkCmdBindDescriptorSets(
    m_handle,
    pipeline->getBindPoint(),
    CRUDE_OBJECT_HANDLE(pipeline->getPipelineLayout()),
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
    CRUDE_OBJECT_HANDLE(m_device), 
    CRUDE_OBJECT_HANDLE(m_commandPool), 
    1u, 
    &m_handle);
}

}