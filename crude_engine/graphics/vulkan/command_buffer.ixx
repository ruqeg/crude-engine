module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.command_buffer;

import crude_engine.core.shared_ptr;
import crude_engine.core.std_containers_stack;
import crude_engine.graphics.vulkan.vulkan_object;
import crude_engine.graphics.vulkan.image_memory_barrier;

export namespace crude_engine
{

class Device;
class Command_Pool;
class Buffer;
class Image;
class Render_Pass;
class Framebuffer;
class Pipeline;
class Descriptor_Set;


class Command_Buffer : public Vulkan_Object<VkCommandBuffer>
{
public:
  explicit Command_Buffer(Shared_Ptr<const Device>  device,
                          Shared_Ptr<Command_Pool>  commandPool,
                          VkCommandBufferLevel      level);

  bool begin(VkCommandBufferUsageFlags flags = 0u);

  bool end();
  
  void barrier(VkPipelineStageFlags        srcStage, 
               VkPipelineStageFlags        dstStage, 
               span<Image_Memory_Barrier>  imageMemoryBarriers);
  
  void copyBufferToImage(Shared_Ptr<Buffer>       srcBuffer, 
                         Shared_Ptr<Image>        dstImage, 
                         span<VkBufferImageCopy>  regions);
  
  bool reset(VkCommandBufferResetFlags flags = VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

  void beginRenderPass(Shared_Ptr<Render_Pass>  renderPass,
                       Shared_Ptr<Framebuffer>  framebuffer,
                       span<VkClearValue>       clearValues,
                       const VkRect2D&          renderArea = {0, 0, 0, 0}, 
                       VkSubpassContents        contents = VK_SUBPASS_CONTENTS_INLINE);

  void bindPipeline(Shared_Ptr<Pipeline> pipeline);

  void setViewport(span<VkViewport> viewports);

  void setScissor(span<VkRect2D> scissors);

  void bindDescriptorSets(Shared_Ptr<Pipeline>              pipeline,
                          span<Shared_Ptr<Descriptor_Set>>  descriptorSets, 
                          span<uint32>                      dynamicOffsets = {});

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
