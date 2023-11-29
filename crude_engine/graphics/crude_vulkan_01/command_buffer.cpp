#include "command_buffer.hpp"
#include "command_pool.hpp"
#include "device.hpp"
#include "image.hpp"
#include "buffer.hpp"
#include "render_pass.hpp"
#include "framebuffer.hpp"
#include "pipeline.hpp"
#include "pipeline_layout.hpp"
#include "descriptor_set.hpp"

namespace crude_vulkan_01
{

Command_Buffer_Create_Info::Command_Buffer_Create_Info(std::shared_ptr<const Device>  device,
                                                       std::shared_ptr<Command_Pool>  commandPool,
                                                       VkCommandBufferLevel           level)
  :
  device(device),
  commandPool(commandPool),
  level(level)
{}

Command_Buffer::Command_Buffer(const Command_Buffer_Create_Info& createInfo)
{
  m_device       = createInfo.device;
  m_commandPool  = createInfo.commandPool;

  VkCommandBufferAllocateInfo vkAllocateInfo{};
  vkAllocateInfo.sType               = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  vkAllocateInfo.pNext               = nullptr;

  vkAllocateInfo.commandPool         = CRUDE_VULKAN_01_HANDLE(m_commandPool);
  vkAllocateInfo.level               = createInfo.level;
  vkAllocateInfo.commandBufferCount  = 1u;

  VkResult result = vkAllocateCommandBuffers(CRUDE_VULKAN_01_HANDLE(m_device), &vkAllocateInfo, &m_handle);
  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to allocate command buffer");
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

void Command_Buffer::barrier(VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage, const std::vector<Image_Memory_Barrier>& imageMemoryBarriers)
{
  std::vector<VkImageMemoryBarrier> vkImageMemoryBarriers(imageMemoryBarriers.begin(), imageMemoryBarriers.end());
  vkCmdPipelineBarrier(m_handle, srcStage, dstStage, 0u, 0u, nullptr, 0u, nullptr, vkImageMemoryBarriers.size(), vkImageMemoryBarriers.data());

  for (uint32 i = 0u; i < imageMemoryBarriers.size(); ++i)
  {
    imageMemoryBarriers[i].m_image->setLayout(imageMemoryBarriers[i].newLayout);
  }
}

void Command_Buffer::copyBufferToImage(std::shared_ptr<Buffer> srcBuffer, std::shared_ptr<Image> dstImage, const std::vector<VkBufferImageCopy>& regions)
{
  vkCmdCopyBufferToImage(m_handle, CRUDE_VULKAN_01_HANDLE(srcBuffer), CRUDE_VULKAN_01_HANDLE(dstImage), dstImage->getLayout(), regions.size(), regions.data());
}

bool Command_Buffer::reset(VkCommandBufferResetFlags flags)
{
  const VkResult result = vkResetCommandBuffer(m_handle, flags);
  return result != VK_ERROR_OUT_OF_DEVICE_MEMORY;
}

void Command_Buffer::beginRenderPass(std::shared_ptr<Render_Pass>      renderPass,
                                     std::shared_ptr<Framebuffer>      framebuffer,
                                     const std::vector<VkClearValue>&  clearValues,
                                     const VkRect2D&                   renderArea, 
                                     VkSubpassContents                 contents)
{
  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType            = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.pNext            = nullptr;
  renderPassInfo.renderPass       = CRUDE_VULKAN_01_HANDLE(renderPass);
  renderPassInfo.framebuffer      = CRUDE_VULKAN_01_HANDLE(framebuffer);
  renderPassInfo.renderArea       = renderArea;
  renderPassInfo.clearValueCount  = static_cast<uint32>(clearValues.size());
  renderPassInfo.pClearValues     = clearValues.data();
  
  vkCmdBeginRenderPass(
    m_handle,
    &renderPassInfo,
    contents);

  m_withinRenderPass = true;
}

void Command_Buffer::bindPipeline(std::shared_ptr<Pipeline> pipeline)
{
  vkCmdBindPipeline(
    m_handle,
    pipeline->getBindPoint(),
    CRUDE_VULKAN_01_HANDLE(pipeline));
}

void Command_Buffer::setViewport(const std::vector<VkViewport>& viewports)
{
  constexpr uint32 offset = 0u;
  vkCmdSetViewport(
    m_handle,
    offset,
    static_cast<uint32>(viewports.size()),
    viewports.data());
}

void Command_Buffer::setScissor(const std::vector<VkRect2D>& scissors)
{
  constexpr uint32 offset = 0u;
  vkCmdSetScissor(
    m_handle,
    offset,
    static_cast<uint32>(scissors.size()),
    scissors.data());
}

void Command_Buffer::bindDescriptorSets(std::shared_ptr<Pipeline> pipeline, const std::vector<std::shared_ptr<Descriptor_Set>>& descriptorSets, const std::vector<uint32>& dynamicOffsets)
{
  constexpr uint32 offset = 0u;

  std::vector<VkDescriptorSet> descriptorSetsHandles(descriptorSets.size());
  for (uint32 i = 0; i < descriptorSetsHandles.size(); ++i)
    descriptorSetsHandles[i] = CRUDE_VULKAN_01_HANDLE(descriptorSets[i]);

  vkCmdBindDescriptorSets(
    m_handle,
    pipeline->getBindPoint(),
    CRUDE_VULKAN_01_HANDLE(pipeline->getPipelineLayout()),
    offset,
    static_cast<uint32>(descriptorSetsHandles.size()),
    descriptorSetsHandles.data(),
    static_cast<uint32>(dynamicOffsets.size()),
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
  vkFreeCommandBuffers(CRUDE_VULKAN_01_HANDLE(m_device), CRUDE_VULKAN_01_HANDLE(m_commandPool), 1u, &m_handle);
}

}