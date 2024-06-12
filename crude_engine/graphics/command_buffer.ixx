module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.command_buffer;

import crude.core.std_containers_stack;
import crude.graphics.vulkan_object;
export import crude.graphics.image_memory_barrier;
export import crude.core.std_containers_heap;

export namespace crude::graphics
{

class Device;
class Command_Pool;
class Image;
class Render_Pass;
class Framebuffer;
class Pipeline;
class Buffer;
class Descriptor_Set;
class Model_Buffer;

// !TODO remove this???
struct Vertex_Buffer_Bind
{
  Vertex_Buffer_Bind(core::shared_ptr<Buffer> buffer, VkDeviceSize offset);
  core::shared_ptr<Buffer>  buffer;
  VkDeviceSize              offset;
};

class Command_Buffer : public Vulkan_Object<VkCommandBuffer>
{
public:
  explicit Command_Buffer(core::shared_ptr<const Device>  device,
                          core::shared_ptr<Command_Pool>  commandPool,
                          VkCommandBufferLevel            level);

  bool begin(VkCommandBufferUsageFlags flags = 0u);

  bool end();
  
  void barrier(VkPipelineStageFlags              srcStage, 
               VkPipelineStageFlags              dstStage, 
               core::span<Image_Memory_Barrier>  imageMemoryBarriers);
  
  void copyBufferToImage(core::shared_ptr<Buffer>       srcBuffer, 
                         core::shared_ptr<Image>        dstImage, 
                         core::span<VkBufferImageCopy>  regions);
  
  bool reset(VkCommandBufferResetFlags flags = VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

  void beginRenderPass(core::shared_ptr<Render_Pass>  renderPass,
                       core::shared_ptr<Framebuffer>  framebuffer,
                       core::span<VkClearValue>       clearValues,
                       const VkRect2D&                renderArea = {0, 0, 0, 0}, 
                       VkSubpassContents              contents = VK_SUBPASS_CONTENTS_INLINE);

  void bindPipeline(core::shared_ptr<Pipeline> pipeline);

  void setViewport(core::span<VkViewport> viewports);

  void setScissor(core::span<VkRect2D> scissors);
  
  void bindModelBuffer(core::shared_ptr<Model_Buffer> modelBuffer, core::uint32 vertexBinding);
  void bindVertexBuffers(core::uint32 firstBinding, const core::span<Vertex_Buffer_Bind>& vertexBuffersBind);
  void bindVertexBuffer(core::shared_ptr<Buffer> vertexBuffer, core::uint32 firstBinding, VkDeviceSize offset = 0u);
  void bindIndexBuffer(core::shared_ptr<Buffer> indexBuffer, VkIndexType indexType, VkDeviceSize offset = 0u);

  void copyBuffer(core::shared_ptr<const Buffer>  srcBuffer, 
                  core::shared_ptr<Buffer>        dstBuffer, 
                  const VkBufferCopy&             copyRegion);

  // !TODO
  //template<core::size_t N>
  //void bindVertexBuffers(core::uint32 firstBinding, const core::array<Vertex_Buffer_Bind, N>& vertexBuffersBind);

  void bindDescriptorSets(core::shared_ptr<Pipeline>                    pipeline,
                          core::span<core::shared_ptr<Descriptor_Set>>  descriptorSets,
                          core::span<core::uint32>                      dynamicOffsets = {});

  void draw(core::uint32 vertexCount, core::uint32 instanceCount = 0u, core::uint32 firstVertex = 0u, core::uint32 firstInstance = 0u);
  void drawIndexed(core::uint32 indexCount, core::uint32 instanceCount, core::uint32 firstIndex = 0u, core::int32 vertexOffset = 0u, core::uint32 firstInstance = 0u);

  void endRenderPass();
  // !TODO void begin(VkCommandBufferUsageFlags flags with VkCommandBufferInheritanceInfo);
  ~Command_Buffer();
public:
  core::shared_ptr<const Device> getDevice() const;
private:
  const core::shared_ptr<const Device>  m_device;
  const core::shared_ptr<Command_Pool>  m_commandPool;
  bool                                  m_withinRenderPass = false;
  bool                                  m_recording = false;
};

}
