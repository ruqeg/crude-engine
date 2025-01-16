module;

#include <vulkan/vulkan.h>

export module crude.gfx.vk.acceleration_structure;

export import crude.gfx.vk.acceleration_structure_geometry;
export import crude.core.std_containers_heap;
export import crude.core.std_containers_stack;
export import crude.gfx.vk.vulkan_object;

export namespace crude::gfx::vk
{
class Device;
class Buffer;
class Acceleration_Structure_Storage_Buffer;
}

export namespace crude::gfx::vk
{

class Acceleration_Structure : public Vulkan_Object<VkAccelerationStructureKHR>
{
public:
  core::shared_ptr<Acceleration_Structure_Storage_Buffer> getStructureStorageBuffer();
  core::uint64 getReference();
  VkAccelerationStructureTypeKHR getType() const;
  VkBuildAccelerationStructureFlagsKHR getBuildFlags() const;
  VkDeviceSize getBuildScratchSize();
protected:
  explicit Acceleration_Structure(core::shared_ptr<Device>                           device,
                                  VkAccelerationStructureTypeKHR                     structureType,
                                  VkAccelerationStructureCreateFlagsKHR              flags,
                                  VkAccelerationStructureBuildTypeKHR                buildType,
                                  VkBuildAccelerationStructureFlagsKHR               buildFlags,
                                  core::span<const Acceleration_Structure_Geometry>  geometries);
private:
  core::shared_ptr<Device>                                 m_device;
  core::shared_ptr<Acceleration_Structure_Storage_Buffer>  m_buffer;
  VkAccelerationStructureTypeKHR                           m_type;
  VkBuildAccelerationStructureFlagsKHR                     m_buildFlags;
  VkDeviceSize                                             m_buildScratchSize;
};

}
