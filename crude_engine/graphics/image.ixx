module;

#include <vulkan/vulkan.hpp>
#include <memory>

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

class Image : public Vulkan_Object<VkImage>,
  public std::enable_shared_from_this<Image>
{
public:
  struct Initialize_Unique;
  struct Initialize_Handle;
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
                 core::uint32                    mipLevelsCount,
                 core::uint32                    arrayLayersCount,
                 VkSampleCountFlagBits           samples,
                 VkImageCreateFlags              flags,
                 VkImageUsageFlags               usage,
                 VkImageTiling                   tiling,
                 VkSharingMode                   sharingMode);
public:
  ~Image();
  void fillLayout(VkImageLayout newLayout);
  void transitionMipLayout(core::shared_ptr<Command_Buffer> commandBuffer, core::uint32 mipLevel, VkImageLayout newLayout);
  void transitionMipLayoutUpload(core::shared_ptr<Command_Buffer> commandBuffer, core::uint32 mipLevel, VkImageLayout newLayout);
  void bindMemory(core::shared_ptr<Device_Memory> memory);
  void copyMipFromBuffer(core::shared_ptr<Command_Buffer>        commandBuffer,
                         core::shared_ptr<const Staging_Buffer>  srcBuffer,
                         const Copy_Layout&                      bufferLayout,
                         core::uint32                            mipLevel,
                         core::uint32                            arrayLayer,
                         VkImageLayout                           dstLayout,
                         VkPipelineStageFlags                    dstStageMask);
  void stagedUpload(core::shared_ptr<Command_Buffer>  commandBuffer,
                    const Mip_Data&                   mipMap,
                    core::uint32                      mipLevel,
                    core::uint32                      arrayLayer,
                    VkImageLayout                     dstLayout,
                    VkPipelineStageFlags              dstStageMask);
  void setMipLayout(core::uint32 mipLevel, VkImageLayout layout);
  VkImageLayout getMipLayout(core::uint32 mipLevel) const;
  VkImageLayout getLayout() const;
  VkImageType getType() const { return m_type; }
  VkFormat getFormat() const { return m_format; }
  core::uint32 getMipLevelsCount() const { return m_mipLevelsCount; }
  core::uint32 getArrayLayersCount() const { return m_arrayLayersCount; }
  const VkExtent3D& getExtent() const { return m_extent; }
  VkExtent2D getExtent2D() const { return VkExtent2D{ m_extent.width, m_extent.height }; }
  core::uint32 getWidth() const { return m_extent.width; }
  core::uint32 getHeight() const { return m_extent.height; }
  core::uint32 getDepth() const { return m_extent.depth; }
  VkImageUsageFlags getUsage() const { return m_usage; }
  VkImageCreateFlags getFlags() const { return m_flags; }
  VkSampleCountFlagBits getSampleCount() const { return m_samples; }
  VkMemoryRequirements getMemoryRequirements() const;
  core::shared_ptr<const Device> getDevice() const;
  VkExtent3D calculateMipExtent(core::uint32 mipLevel) const;
  core::uint32 calculateMipWidth(core::uint32 mipLevel) const;
  core::uint32 calculateMipHeight(core::uint32 mipLevel) const;
protected:
  const core::shared_ptr<const Device>   m_device;
  core::shared_ptr<Device_Memory>        m_memory;
  const VkSampleCountFlagBits            m_samples;
  const VkFormat                         m_format;
  const VkExtent3D                       m_extent;
  const VkImageUsageFlags                m_usage;
  const VkImageType                      m_type;
  const VkImageCreateFlags               m_flags;
  core::vector<VkImageLayout>            m_layouts;
  const core::uint32                     m_mipLevelsCount;
  const core::uint32                     m_arrayLayersCount;
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
