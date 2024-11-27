module;

#include <algorithm>
#include <vulkan/vulkan.h>
#include <memory>

export module crude.gfx.vk.buffer;

export import crude.gfx.vk.device_memory;
export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.gfx.vk.command_buffer;
import crude.gfx.vk.vulkan_object;

export namespace crude::gfx::vk
{

class Device;
class Device_Memory;
class Staging_Buffer;

class Buffer : public Vulkan_Object<VkBuffer>, 
  public std::enable_shared_from_this<Buffer>
{
public:
  struct Initialize;
public:
  explicit Buffer(const Initialize& info);
  ~Buffer();
public:
  template<class T>
  void stagedUpload(core::shared_ptr<Command_Buffer> commandBuffer, core::span<const T> data) noexcept;
  template<class T>
  void copyHost(core::span<const T> data) noexcept;
public:
  void copyTransfer(core::shared_ptr<Command_Buffer>        commandBuffer,
                    core::shared_ptr<const Staging_Buffer>  srcBuffer,
                    VkDeviceSize                            srcOffset = 0,
                    VkDeviceSize                            dstOffset = 0,
                    VkDeviceSize                            size = VK_WHOLE_SIZE);
  void bindMemory(core::shared_ptr<Device_Memory> memory, VkDeviceSize offset = 0);
  void stagedUpload(core::shared_ptr<Command_Buffer> commandBuffer, const void* data, VkDeviceSize size) noexcept;
  void copyHost(const void* data, VkDeviceSize size) noexcept;
  VkMemoryRequirements getMemoryRequirements() const;
  VkDeviceSize getSize() const;
protected:
  core::shared_ptr<const Device>   m_device;
  core::shared_ptr<Device_Memory>  m_memory;
  VkDeviceSize                     m_size;
};

template<class T>
void Buffer::stagedUpload(core::shared_ptr<Command_Buffer> commandBuffer, core::span<const T> data) noexcept
{
  stagedUpload(commandBuffer, data.data(), data.size_bytes());
}

template<class T>
void Buffer::copyHost(core::span<const T> data) noexcept
{
  copyHost(data.data(), data.size_bytes());
}

struct Buffer::Initialize
{
  core::shared_ptr<const Device>  device;
  VkDeviceSize                    size;
  VkBufferUsageFlags              usage;
  VkMemoryPropertyFlags           memoryFlags;
  core::span<const core::uint32>  queueFamilyIndices = {};
};

}
