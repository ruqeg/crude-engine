module;

#include <vulkan/vulkan.h>

export module crude.gfx.vk.acceleration_structure;

export import crude.gfx.vk.acceleration_structure_geometry_triangles;
export import crude.core.std_containers_heap;
export import crude.core.std_containers_stack;
export import crude.gfx.vk.vulkan_object;

export namespace crude::gfx::vk
{
class Device;
class Buffer;
}

export namespace crude::gfx::vk
{

class Acceleration_Structure : public Vulkan_Object<VkAccelerationStructureKHR>
{
protected:
  explicit Acceleration_Structure(core::shared_ptr<Device>                                     device,
                                  VkAccelerationStructureTypeKHR                               structureType,
                                  VkAccelerationStructureCreateFlagsKHR                        flags,
                                  VkAccelerationStructureBuildTypeKHR                          buildType,
                                  VkBuildAccelerationStructureFlagsKHR                         buildFlags,
                                  core::span<const Acceleration_Structure_Geometry_Triangles>  geometries);
private:
  core::shared_ptr<Device> m_device;
};

}
