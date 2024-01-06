#pragma once

#include <graphics/vulkan/object.hpp>
#include <graphics/vulkan/image_memory_barrier.hpp>

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


class Command_Buffer : public TObject<VkCommandBuffer>
{
public:
  explicit Command_Buffer(Shared_Ptr<const Device>  device,
                          Shared_Ptr<Command_Pool>  commandPool,
                          VkCommandBufferLevel      level);

  bool begin(VkCommandBufferUsageFlags flags = 0u);

  bool end();
  
  void barrier(VkPipelineStageFlags  srcStage, 
               VkPipelineStageFlags  dstStage, 
               Image_Memory_Barrier* pImageMemoryBarriers, 
               uint32                imageMemoryBarrierCount);
  
  void copyBufferToImage(Buffer*            pSrcBuffer, 
                         Image*             pDstImage, 
                         VkBufferImageCopy* pRegions, 
                         uint32             regionCount);
  
  bool reset(VkCommandBufferResetFlags flags = VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

  void beginRenderPass(Render_Pass*       pRenderPass, 
                       Framebuffer*       pFramebuffer,
                       VkClearValue*      pClearValues,
                       uint32             clearValueCount,
                       const VkRect2D&    renderArea = {0, 0, 0, 0}, 
                       VkSubpassContents  contents = VK_SUBPASS_CONTENTS_INLINE);

  void bindPipeline(Pipeline* pPipeline);

  void setViewport(VkViewport* pViewports, uint32 viewportCount);

  void setScissor(VkRect2D* pScissors, uint32 scissorCount);

  void bindDescriptorSets(Pipeline*         pPipeline, 
                          Descriptor_Set**  pDescriptorSets, 
                          uint32            descriptorSetCount, 
                          uint32*           pDynamicOffsets,
                          uint32            dynamicOffsetCount);

  void draw(uint32 vertexCount, uint32 instanceCount = 0u, uint32 firstVertex = 0u, uint32 firstInstance = 0u);

  void endRenderPass();
  // !TODO void begin(VkCommandBufferUsageFlags flags with VkCommandBufferInheritanceInfo);
  ~Command_Buffer();
private:
  const Shared_Ptr<const Device>  m_device;
  const Shared_Ptr<Command_Pool>  m_commandPool;
  bool                            m_withinRenderPass = false;
  bool                            m_recording = false;
};

}
