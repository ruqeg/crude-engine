#pragma once

#include "image_memory_barrier.hpp"
#include "object.hpp"
#include <vector>

namespace crude_engine
{

class Device;
class Command_Pool;
class Buffer;
class Image;
class Render_Pass;
class Framebuffer;
class Pipeline;
class Descriptor_Set;

struct Command_Buffer_Create_Info
{
  std::shared_ptr<const Device>  device;
  std::shared_ptr<Command_Pool>  commandPool;
  VkCommandBufferLevel           level;
  explicit Command_Buffer_Create_Info(std::shared_ptr<const Device>  device,
                                      std::shared_ptr<Command_Pool>  commandPool,
                                      VkCommandBufferLevel           level);
};


class Command_Buffer : public TObject<VkCommandBuffer>
{
public:
  explicit Command_Buffer(const Command_Buffer_Create_Info& createInfo);
  bool begin(VkCommandBufferUsageFlags flags = 0u);
  bool end();
  void barrier(VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage, const std::vector<Image_Memory_Barrier>& imageMemoryBarriers);
  void copyBufferToImage(std::shared_ptr<Buffer> srcBuffer, std::shared_ptr<Image> dstImage, const std::vector<VkBufferImageCopy>& regions);
  bool reset(VkCommandBufferResetFlags flags = VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
  void beginRenderPass(std::shared_ptr<Render_Pass>      renderPass, 
                       std::shared_ptr<Framebuffer>      framebuffer,
                       const std::vector<VkClearValue>&  clearValues,
                       const VkRect2D&                   renderArea = {0, 0, 0, 0}, 
                       VkSubpassContents                 contents = VK_SUBPASS_CONTENTS_INLINE);
  void bindPipeline(std::shared_ptr<Pipeline> pipeline);
  void setViewport(const std::vector<VkViewport>& viewports);
  void setScissor(const std::vector<VkRect2D>& scissors);
  void bindDescriptorSets(std::shared_ptr<Pipeline> pipeline, const std::vector<std::shared_ptr<Descriptor_Set>>& descriptorSets, const std::vector<uint32>& dynamicOffsets = {});
  void draw(uint32 vertexCount, uint32 instanceCount = 0u, uint32 firstVertex = 0u, uint32 firstInstance = 0u);
  void endRenderPass();
  // !TODO void begin(VkCommandBufferUsageFlags flags with VkCommandBufferInheritanceInfo);
  ~Command_Buffer();
private:
  std::shared_ptr<const Device>  m_device;
  std::shared_ptr<Command_Pool>  m_commandPool;
  bool                           m_withinRenderPass = false;
  bool                           m_recording = false;
};

}
