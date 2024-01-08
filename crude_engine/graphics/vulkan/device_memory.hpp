#pragma once

#include <core/data_structures/shared_ptr.hpp>
#include <core/data_structures/optional.hpp>
#include <graphics/vulkan/include_vulkan.hpp>
#include <graphics/vulkan/object.hpp>

namespace crude_engine
{

class Device;
class Image;
class Buffer;

class Device_Memory : public TObject<VkDeviceMemory>
{
public:
  explicit Device_Memory(Shared_Ptr<const Device>  device,
                         VkDeviceSize              allocationSize,
                         uint32                    memoryTypeIndex);

  explicit Device_Memory(Shared_Ptr<const Device>  device,
                         VkDeviceSize              allocationSize,
                         uint32                    memoryTypeFilter,
                         VkMemoryPropertyFlags     memoryProperties);
  ~Device_Memory();
  void bind(Image& image, VkDeviceSize offset = 0u);
  void bind(Buffer& buffer, VkDeviceSize offset = 0u);
  Optional<void*> map(VkDeviceSize offset = 0u, VkDeviceSize size = VK_WHOLE_SIZE, VkMemoryMapFlags flags = 0);
  void unmap();
private:
  void initalize(VkDeviceSize allocationSize, uint32 memoryTypeIndex);
private:
  Shared_Ptr<const Device>  m_device;
};

}
