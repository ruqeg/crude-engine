#include <vulkan/vulkan.hpp>
#undef assert

module crude.gfx.vk.command_buffer;

import crude.gfx.vk.command_pool;
import crude.gfx.vk.device;
import crude.gfx.vk.image;
import crude.gfx.vk.render_pass;
import crude.gfx.vk.framebuffer;
import crude.gfx.vk.acceleration_structure;
import crude.gfx.vk.storage_buffer;
import crude.gfx.vk.storage_buffer;
import crude.gfx.vk.pipeline;
import crude.gfx.vk.pipeline_layout;
import crude.gfx.vk.image_view;
import crude.gfx.vk.buffer;
import crude.gfx.vk.vertex_buffer;
import crude.gfx.vk.index_buffer;
import crude.gfx.vk.descriptor_set;
import crude.gfx.vk.vulkan_utils;
import crude.core.std_containers_heap;
import crude.core.algorithms;
import crude.core.assert;
import crude.gfx.vk.extension;
import crude.core.logger;
import crude.gfx.vk.acceleration_structure_geometry;

namespace crude::gfx::vk
{

Command_Buffer::Command_Buffer(core::shared_ptr<Command_Pool> commandPool, VkCommandBufferLevel level)
  : m_commandPool(commandPool)
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

void Command_Buffer::barrier(VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage, const Image_Memory_Barrier& imageMemoryBarrier)
{
  VkImageMemoryBarrier vkImageMemoryBarrier = imageMemoryBarrier;
  vkCmdPipelineBarrier(m_handle, srcStage, dstStage, 0u, 0u, nullptr, 0u, nullptr, 1u, &vkImageMemoryBarrier);
  const VkImageSubresourceRange& range = imageMemoryBarrier.subresourceRange;
  for (core::uint32 mipLevel = range.baseMipLevel; mipLevel < range.baseMipLevel + range.levelCount; ++mipLevel)
  {
    imageMemoryBarrier.m_image->setMipLayout(mipLevel, imageMemoryBarrier.newLayout);
  }
}

void Command_Buffer::barrier(VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage, const Memory_Barrier& memoryBarrier)
{
  VkMemoryBarrier vkMemoryBarrier = memoryBarrier;
  vkCmdPipelineBarrier(m_handle, srcStage, dstStage, 0u, 1u, &vkMemoryBarrier, 0u, nullptr, 0u, nullptr);
}

void Command_Buffer::blitImage(const Image_Blit_Region&  blitRegion,
                               VkFilter                  filter)
{
  VkImageBlit vkImageBlit = blitRegion;
  vkCmdBlitImage(
    m_handle,
    blitRegion.getSourceImage()->getHandle(), blitRegion.getSourceImage()->getMipLayout(blitRegion.srcSubresource.mipLevel),
    blitRegion.getDestinationImage()->getHandle(), blitRegion.getDestinationImage()->getMipLayout(blitRegion.dstSubresource.mipLevel),
    1, &vkImageBlit,
    filter);
}

void Command_Buffer::copyBufferToImage(core::shared_ptr<const Buffer>  srcBuffer,
                                       core::shared_ptr<Image>         dstImage, 
                                       const VkBufferImageCopy&        region)
{
  vkCmdCopyBufferToImage(
    m_handle, srcBuffer->getHandle(), dstImage->getHandle(), dstImage->getMipLayout(region.imageSubresource.mipLevel),
    1u, &region);
}

bool Command_Buffer::reset(VkCommandBufferResetFlags flags)
{
  const VkResult result = vkResetCommandBuffer(m_handle, flags);
  return result != VK_ERROR_OUT_OF_DEVICE_MEMORY;
}

void Command_Buffer::beginRenderPass(core::shared_ptr<Render_Pass> renderPass, core::shared_ptr<Framebuffer> framebuffer, const VkRect2D& renderArea, VkSubpassContents contents)
{
  beginRenderPass(renderPass, framebuffer, {}, renderArea, contents);
}

void Command_Buffer::beginRenderPass(core::shared_ptr<Render_Pass> renderPass, core::shared_ptr<Framebuffer> framebuffer, core::span<VkClearValue> clearValues, const VkRect2D& renderArea, VkSubpassContents contents)
{
  core::assert(renderPass.get());
  core::assert(framebuffer.get());

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType            = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.pNext            = nullptr;
  renderPassInfo.renderPass       = renderPass->getHandle();
  renderPassInfo.framebuffer      = framebuffer->getHandle();
  renderPassInfo.renderArea       = renderArea;
  renderPassInfo.clearValueCount  = clearValues.size();
  renderPassInfo.pClearValues     = clearValues.data();
  
  vkCmdBeginRenderPass(m_handle, &renderPassInfo, contents);

  m_renderPass = renderPass;
  m_framebuffer = framebuffer;
  m_withinRenderPass = true;
}

void Command_Buffer::bindPipeline(core::shared_ptr<Pipeline> pipeline)
{
  core::assert(pipeline.get());
  vkCmdBindPipeline(m_handle, pipeline->getBindPoint(), pipeline->getHandle());
}

void Command_Buffer::buildAccelerationStructures(core::shared_ptr<Acceleration_Structure> accelerationStructure, core::span<const Acceleration_Structure_Geometry> geometries, core::shared_ptr<Storage_Buffer> scratchBuffer)
{
  core::vector<VkAccelerationStructureGeometryKHR> vkGeometries(geometries.begin(), geometries.end());

  VkAccelerationStructureBuildGeometryInfoKHR buildGeometryInfo{};
  buildGeometryInfo.sType                     = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
  buildGeometryInfo.pNext                     = nullptr;
  buildGeometryInfo.type                      = accelerationStructure->getType();
  buildGeometryInfo.flags                     = accelerationStructure->getBuildFlags();
  buildGeometryInfo.mode                      = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
  buildGeometryInfo.dstAccelerationStructure  = accelerationStructure->getHandle();
  buildGeometryInfo.geometryCount             = vkGeometries.size();
  buildGeometryInfo.pGeometries               = vkGeometries.data();
  buildGeometryInfo.ppGeometries              = nullptr;
  buildGeometryInfo.scratchData.deviceAddress = scratchBuffer->getDeviceAddress();
  buildGeometryInfo.srcAccelerationStructure  = (buildGeometryInfo.mode == VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR) ? VK_NULL_HANDLE : accelerationStructure->getHandle();

  core::vector<VkAccelerationStructureBuildRangeInfoKHR> buildRangeInfos;
  buildRangeInfos.reserve(geometries.size());
  for (const auto& geometry : geometries)
  {
    buildRangeInfos.push_back(VkAccelerationStructureBuildRangeInfoKHR{ .primitiveCount = geometry.primitiveCount });
  }
  const VkAccelerationStructureBuildRangeInfoKHR* buildRangeInfosData = buildRangeInfos.data();

  auto vkCmdBuildAccelerationStructuresKHR = getDeviceExtension<PFN_vkCmdBuildAccelerationStructuresKHR>(m_commandPool->getDevice());
  if (vkCmdBuildAccelerationStructuresKHR)
  {
    vkCmdBuildAccelerationStructuresKHR(m_handle, 1, &buildGeometryInfo, &buildRangeInfosData);
  }
}

void Command_Buffer::setViewport(const Viewport& viewport)
{
  constexpr core::uint32 offset = 0u;
  vkCmdSetViewport(m_handle, offset, 1u, &viewport);
}

void Command_Buffer::setScissor(const Scissor& scissor)
{
  constexpr core::uint32 offset = 0u;
  vkCmdSetScissor(m_handle, offset, 1u, &scissor);
}

void Command_Buffer::bindVertexBuffer(core::uint32 firstBinding, core::shared_ptr<Buffer> vertexBuffer, VkDeviceSize offset)
{
  vkCmdBindVertexBuffers(m_handle, firstBinding, 1u, &vertexBuffer->getHandle(), &offset);
}

void Command_Buffer::bindIndexBuffer(core::shared_ptr<Index_Buffer> indexBuffer, VkDeviceSize offset)
{
  vkCmdBindIndexBuffer(m_handle, indexBuffer->getHandle(), offset, indexBuffer->getIndexType());
}

void Command_Buffer::copyBuffer(core::shared_ptr<const Buffer> srcBuffer, core::shared_ptr<Buffer> dstBuffer, const VkBufferCopy& copyRegion)
{
  vkCmdCopyBuffer(m_handle, srcBuffer->getHandle(), dstBuffer->getHandle(), 1, &copyRegion);
}

void Command_Buffer::bindDescriptorSets(core::shared_ptr<Pipeline> pipeline, core::span<const core::shared_ptr<Descriptor_Set>> descriptorSets, core::span<const core::uint32> dynamicOffsets)
{
  constexpr core::uint32 offset = 0u;

  core::vector<VkDescriptorSet> descriptorSetsHandles(descriptorSets.size());
  core::copyc(descriptorSets.begin(), descriptorSets.end(), descriptorSetsHandles.begin(), [](auto& src, auto& dst) -> void {
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

void Command_Buffer::bindDescriptorSet(core::shared_ptr<Pipeline> pipeline, core::shared_ptr<Descriptor_Set> descriptorSet, core::uint32 dynamicOffset)
{
  constexpr core::uint32 offset = 0u;
  vkCmdBindDescriptorSets(
    m_handle,
    pipeline->getBindPoint(),
    pipeline->getPipelineLayout()->getHandle(),
    offset,
    1u,
    &descriptorSet->getHandle(),
    1u,
    &dynamicOffset);
}

void Command_Buffer::pushDescriptorSet(core::shared_ptr<Pipeline> pipeline, core::span<const VkWriteDescriptorSet> descriptorWrites)
{
  auto vkCmdPushDescriptorSetKHR = getDeviceExtension<PFN_vkCmdPushDescriptorSetKHR>(m_commandPool->getDevice());
  if (vkCmdPushDescriptorSetKHR)
  {
    vkCmdPushDescriptorSetKHR(
      m_handle,
      pipeline->getBindPoint(),
      pipeline->getPipelineLayout()->getHandle(),
      0u,
      descriptorWrites.size(),
      descriptorWrites.data());
  }
}

void Command_Buffer::pushConstantBase(core::shared_ptr<const Pipeline_Layout> layout, core::span<const core::byte> data, core::uint32 offset)
{
  if (layout->m_pushConstantRanges.size() < 1)
  {
    core::logError(core::Debug::Channel::Graphics, "Failed push constant (layout->m_pushConstantRanges.size() < 1)");
    return;
  }
  vkCmdPushConstants(m_handle, layout->getHandle(), layout->m_pushConstantRanges.front().stageFlags, offset, data.size_bytes(), data.data());
}

void Command_Buffer::draw(core::uint32 vertexCount, core::uint32 instanceCount, core::uint32 firstVertex, core::uint32 firstInstance)
{
  vkCmdDraw(m_handle, vertexCount, instanceCount, firstVertex, firstInstance);
}

void Command_Buffer::drawIndexed(core::uint32 indexCount, core::uint32 instanceCount, core::uint32 firstIndex, core::int32 vertexOffset, core::uint32 firstInstance)
{
  vkCmdDrawIndexed(m_handle, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void Command_Buffer::drawMeshTasks(core::uint32 groupCountX, core::uint32 groupCountY, core::uint32 groupCountZ)
{
  auto vkCmdDrawMeshTasksEXT = getDeviceExtension<PFN_vkCmdDrawMeshTasksEXT>(m_commandPool->getDevice());
  if (vkCmdDrawMeshTasksEXT)
  {
    vkCmdDrawMeshTasksEXT(m_handle, groupCountX, groupCountY, groupCountZ);
  }
}

void Command_Buffer::endRenderPass()
{
  if (m_withinRenderPass)
  {
    vkCmdEndRenderPass(m_handle);

    if (m_framebuffer.has_value() && m_renderPass.has_value())
    {
      const core::vector<Attachment_Description>& attachmentsDescs = m_renderPass.value()->m_attachmentsDesc;
      const core::vector<core::shared_ptr<Image_View>>& attachments = m_framebuffer.value()->m_attachments;

      for (core::uint32 attachmentIndex = 0; attachmentIndex < attachments.size(); ++attachmentIndex)
      {
        const core::shared_ptr<Image>& image = attachments[attachmentIndex]->getImage();
        image->fillLayout(attachmentsDescs[attachmentIndex].finalLayout);
      }

      m_framebuffer->reset();
      m_renderPass->reset();
    }

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