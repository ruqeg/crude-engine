module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.command_buffer;

import crude.core.std_containers_stack;
import crude.gfx.vk.vulkan_object;
export import crude.gfx.vk.image_memory_barrier;
export import crude.gfx.vk.memory_barrier;
export import crude.core.std_containers_heap;
export import crude.core.std_containers_stack;
export import crude.gfx.vk.scissor;
export import crude.gfx.vk.viewport;
export import crude.gfx.vk.image_blit_region;

export namespace crude::gfx::vk
{

class Device;
class Command_Pool;
class Image;
class Render_Pass;
class Framebuffer;
class Pipeline;
class Buffer;
class Descriptor_Set;
class Index_Buffer;
class Pipeline_Layout;
class Storage_Buffer;
class Acceleration_Structure;
class Acceleration_Structure_Geometry;

class Command_Buffer : public Vulkan_Object<VkCommandBuffer>
{
public:
  explicit Command_Buffer(core::shared_ptr<Command_Pool> commandPool, VkCommandBufferLevel level);
  ~Command_Buffer();
public:
  bool begin(VkCommandBufferUsageFlags flags = 0u);
  bool end();
  void barrier(VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage, const Image_Memory_Barrier& imageMemoryBarrier);
  void barrier(VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage, const Memory_Barrier& memoryBarrier);
  void blitImage(const Image_Blit_Region& blitRegion, VkFilter filter);
  void copyBufferToImage(core::shared_ptr<const Buffer> srcBuffer, core::shared_ptr<Image> dstImage, const VkBufferImageCopy& region);
  bool reset(VkCommandBufferResetFlags flags = VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
  void beginRenderPass(core::shared_ptr<Render_Pass> renderPass, core::shared_ptr<Framebuffer> framebuffer, const VkRect2D& renderArea = {0, 0, 0, 0}, VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE);
  void beginRenderPass(core::shared_ptr<Render_Pass> renderPass, core::shared_ptr<Framebuffer> framebuffer, core::span<VkClearValue> clearValues, const VkRect2D& renderArea = {0, 0, 0, 0}, VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE);
  void bindPipeline(core::shared_ptr<Pipeline> pipeline);
  void buildAccelerationStructures(core::shared_ptr<Acceleration_Structure> accelerationStructure, core::span<const Acceleration_Structure_Geometry> geometries, core::shared_ptr<Storage_Buffer> scratchBuffer);
  void setViewport(const Viewport& viewport);
  void setScissor(const Scissor& scissor);
  void bindVertexBuffer(core::uint32 firstBinding, core::shared_ptr<Buffer> vertexBuffer, VkDeviceSize offset = 0u);
  void bindIndexBuffer(core::shared_ptr<Index_Buffer> indexBuffer, VkDeviceSize offset = 0u);
  void copyBuffer(core::shared_ptr<const Buffer> srcBuffer, core::shared_ptr<Buffer> dstBuffer, const VkBufferCopy& copyRegion);
  void pushDescriptorSet(core::shared_ptr<Pipeline> pipeline, core::span<const VkWriteDescriptorSet> descriptorWrites);
  void bindDescriptorSets(core::shared_ptr<Pipeline> pipeline, core::span<const core::shared_ptr<Descriptor_Set>> descriptorSets, core::span<const core::uint32> dynamicOffsets = {});
  void bindDescriptorSet(core::shared_ptr<Pipeline> pipeline, core::shared_ptr<Descriptor_Set> descriptorSet, core::uint32 dynamicOffset = 0u);
  template<class T>
  void pushConstant(core::shared_ptr<const Pipeline_Layout> layout, const T& constant, core::uint32 offset = 0);
  void pushConstantBase(core::shared_ptr<const Pipeline_Layout> layout, core::span<const core::byte> data, core::uint32 offset);
  void draw(core::uint32 vertexCount, core::uint32 instanceCount = 0u, core::uint32 firstVertex = 0u, core::uint32 firstInstance = 0u);
  void drawIndexed(core::uint32 indexCount, core::uint32 instanceCount, core::uint32 firstIndex = 0u, core::int32 vertexOffset = 0u, core::uint32 firstInstance = 0u);
  void drawMeshTasks(core::uint32 groupCountX, core::uint32 groupCountY = 1u, core::uint32 groupCountZ = 1u);
  void endRenderPass();
public:
  core::shared_ptr<const Device> getDevice() const;
  core::shared_ptr<Command_Pool> getCommandPool();
private:
  const core::shared_ptr<Command_Pool>          m_commandPool;
  core::optional<core::shared_ptr<Render_Pass>> m_renderPass;
  core::optional<core::shared_ptr<Framebuffer>> m_framebuffer;
  bool                                          m_withinRenderPass = false;
  bool                                          m_recording = false;
};

template<class T>
void Command_Buffer::pushConstant(core::shared_ptr<const Pipeline_Layout> layout, const T& constant, core::uint32 offset)
{
  pushConstantBase(layout, core::span<const core::byte>(reinterpret_cast<const core::byte*>(&constant), sizeof(T)), offset);
}

}
