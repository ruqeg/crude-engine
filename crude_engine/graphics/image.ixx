module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.image;

export import crude.core.std_containers_heap;
import crude.graphics.vulkan_object;

export namespace crude::graphics
{

class Command_Buffer;
class Device;
class Device_Memory;

class Image : public Vulkan_Object<VkImage>
{
protected:
  explicit Image(core::shared_ptr<const Device>  device,
                 VkImage                         handle,
                 VkImageType                     type,
                 VkFormat                        format,
                 VkExtent3D                      extent);

  explicit Image(core::shared_ptr<const Device>  device,
                 VkImageType                     type,
                 VkFormat                        format,
                 const VkExtent3D&               extent,
                 core::uint32                    mipLevels,
                 core::uint32                    arrayLayers,
                 VkSampleCountFlagBits           samples,
                 VkImageCreateFlags              flags,
                 VkImageUsageFlags               usage,
                 VkImageTiling                   tiling,
                 VkSharingMode                   sharingMode);
public:
  ~Image();
  void layoutTransition(core::shared_ptr<Command_Buffer> commandBuffer, VkImageLayout newLayout);
  void layoutTransitionUpload(core::shared_ptr<Command_Buffer> commandBuffer, VkImageLayout newLayout);
  void bindMemory(core::shared_ptr<Device_Memory> memory);
  void setLayout(VkImageLayout layout);
  VkImageType getType() const { return m_type; }
  VkFormat getFormat() const { return m_format; }
  VkImageLayout getLayout() const { return m_layout; }
  VkMemoryRequirements getMemoryRequirements() const;
  core::shared_ptr<const Device> getDevice() const;
protected:
  core::shared_ptr<const Device>   m_device;
  core::shared_ptr<Device_Memory>  m_memory;
  VkFormat                         m_format;
  VkExtent3D                       m_extent;
  VkImageUsageFlags                m_usage;
  VkImageType                      m_type;
  VkImageLayout                    m_layout;
};

}
