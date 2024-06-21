module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.image;

export import crude.core.std_containers_heap;
export import crude.core.std_containers_stack;
import crude.graphics.vulkan_object;

export namespace crude::graphics
{

class Command_Buffer;
class Device;
class Device_Memory;
class Staging_Buffer;

class Image : public Vulkan_Object<VkImage>
{
public:
  struct Mip;
  struct Mip_Data;
  struct Copy_Layout;
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
  void copyMip(core::shared_ptr<Command_Buffer>        commandBuffer,
               core::shared_ptr<const Staging_Buffer>  srcBuffer,
               const Copy_Layout&                      bufferLayout,
               core::uint32                            mipLevel,
               core::uint32                            arrayLayer,
               VkImageLayout                           dstLayout,
               VkPipelineStageFlags                    dstStageMask);

public:
  void stagedUpload(core::shared_ptr<Command_Buffer>  commandBuffer, 
                    const Mip_Data&                   mipMap,
                    core::uint32                      mipLevel,
                    core::uint32                      arrayLayer,
                    VkImageLayout                     dstLayout,
                    VkPipelineStageFlags              dstStageMask);
  void setLayout(VkImageLayout layout);
  VkImageType getType() const { return m_type; }
  VkFormat getFormat() const { return m_format; }
  VkImageLayout getLayout() const { return m_layout; }
  core::uint32 getMipLevels() const { return m_mipLevels; }
  core::uint32 getArrayLayers() const { return m_arrayLayers; }
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
  core::uint32                     m_mipLevels;
  core::uint32                     m_arrayLayers;
};

struct Image::Copy_Layout
{
  VkDeviceSize offset = 0u;
  core::uint32 rowLength = 0u;
  core::uint32 imageHeight = 0u;
};

struct Image::Mip
{
  Mip() noexcept;
  Mip(const VkExtent3D& extent, VkDeviceSize bufferOffset = 0u) noexcept
    : extent(extent), bufferOffset(bufferOffset) {}
  
  VkExtent3D    extent;
  VkDeviceSize  bufferOffset;
};

struct Image::Mip_Data
{
  Mip_Data() noexcept 
    : extent({}), texels() {}
  Mip_Data(const VkExtent3D& extent, core::span<const core::byte> texels) noexcept
    : extent(extent), texels(texels) {}

  VkExtent3D                    extent;
  core::span<const core::byte>  texels;
};

}
