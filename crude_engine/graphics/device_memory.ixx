module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.device_memory;

export import crude.core.std_containers_heap;
export import crude.core.optional;
import crude.graphics.vulkan_object;

export namespace crude::graphics
{

class Device;
class Image;
class Buffer;

class Device_Memory : public Vulkan_Object<VkDeviceMemory>
{
public:
  explicit Device_Memory(core::shared_ptr<const Device>  device,
                         VkDeviceSize                    allocationSize,
                         core::uint32                    memoryTypeIndex);
  
  explicit Device_Memory(core::shared_ptr<const Device>  device,
                         VkMemoryRequirements            memoryRequirements,
                         VkMemoryPropertyFlags           memoryProperties);

  explicit Device_Memory(core::shared_ptr<const Device>  device,
                         VkDeviceSize                    allocationSize,
                         core::uint32                    memoryTypeFilter,
                         VkMemoryPropertyFlags           memoryProperties);
  ~Device_Memory();

  void bind(core::shared_ptr<Image> image, VkDeviceSize offset = 0u);
  void bind(core::shared_ptr<Buffer> buffer, VkDeviceSize offset = 0u);
  core::Optional<void*> map(VkDeviceSize offset = 0u, VkDeviceSize size = VK_WHOLE_SIZE, VkMemoryMapFlags flags = 0);
  void* mapUnsafe(VkDeviceSize offset = 0u, VkDeviceSize size = VK_WHOLE_SIZE, VkMemoryMapFlags flags = 0);
  void unmap();
  core::shared_ptr<const Device> getDevice() const;
private:
  void initalize(VkDeviceSize allocationSize, core::uint32 memoryTypeIndex);
private:
  core::shared_ptr<const Device>  m_device;
};

}
