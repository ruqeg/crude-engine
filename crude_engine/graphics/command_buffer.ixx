module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.vulkan.command_buffer;

import crude.core.shared_ptr;
import crude.core.std_containers_stack;
import crude.graphics.vulkan.vulkan_object;
import crude.graphics.vulkan.image_memory_barrier;
import crude.graphics.vulkan.buffer;

export namespace crude::graphics
{

class Device;
class Command_Pool;
class Image;
class Render_Pass;
class Framebuffer;
class Pipeline;
class Descriptor_Set;


class Command_Buffer : public Vulkan_Object<VkCommandBuffer>
{
public:
  explicit Command_Buffer(core::Shared_Ptr<const Device>  device,
                          core::Shared_Ptr<Command_Pool>  commandPool,
                          VkCommandBufferLevel            level);

  bool begin(VkCommandBufferUsageFlags flags = 0u);

  bool end();
  
  void barrier(VkPipelineStageFlags              srcStage, 
               VkPipelineStageFlags              dstStage, 
               core::span<Image_Memory_Barrier>  imageMemoryBarriers);
  
  void copyBufferToImage(core::Shared_Ptr<Buffer>       srcBuffer, 
                         core::Shared_Ptr<Image>        dstImage, 
                         core::span<VkBufferImageCopy>  regions);
  
  bool reset(VkCommandBufferResetFlags flags = VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

  void beginRenderPass(core::Shared_Ptr<Render_Pass>  renderPass,
                       core::Shared_Ptr<Framebuffer>  framebuffer,
                       core::span<VkClearValue>       clearValues,
                       const VkRect2D&                renderArea = {0, 0, 0, 0}, 
                       VkSubpassContents              contents = VK_SUBPASS_CONTENTS_INLINE);

  void bindPipeline(core::Shared_Ptr<Pipeline> pipeline);

  void setViewport(core::span<VkViewport> viewports);

  void setScissor(core::span<VkRect2D> scissors);

  void bindDescriptorSets(core::Shared_Ptr<Pipeline>                    pipeline,
                          core::span<core::Shared_Ptr<Descriptor_Set>>  descriptorSets,
                          core::span<core::uint32>                      dynamicOffsets = {});

  void draw(core::uint32 vertexCount, core::uint32 instanceCount = 0u, core::uint32 firstVertex = 0u, core::uint32 firstInstance = 0u);

  void endRenderPass();
  // !TODO void begin(VkCommandBufferUsageFlags flags with VkCommandBufferInheritanceInfo);
  ~Command_Buffer();
private:
  const core::Shared_Ptr<const Device>  m_device;
  const core::Shared_Ptr<Command_Pool>  m_commandPool;
  bool                                  m_withinRenderPass = false;
  bool                                  m_recording = false;
};

}
