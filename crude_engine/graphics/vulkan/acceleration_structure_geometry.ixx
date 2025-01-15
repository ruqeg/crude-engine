module;

#include <vulkan/vulkan.h>

export module crude.gfx.vk.acceleration_structure_geometry;

export import crude.core.std_containers_heap;

import crude.gfx.vk.acceleration_structure_instance_buffer;

export namespace crude::gfx::vk
{
class Acceleration_Structure_Input_Buffer;
template<class T>
class Acceleration_Structure_Instance_Buffer;
}

export namespace crude::gfx::vk
{

class Acceleration_Structure_Geometry : public VkAccelerationStructureGeometryKHR
{
public:
  Acceleration_Structure_Geometry(VkGeometryTypeKHR geometryType, VkGeometryFlagsKHR flags = 0) noexcept;
  core::uint32 primitiveCount;
};

class Acceleration_Structure_Geometry_Triangles : public Acceleration_Structure_Geometry
{
public:
  explicit Acceleration_Structure_Geometry_Triangles();
  explicit Acceleration_Structure_Geometry_Triangles(VkFormat vertexFormat, VkDeviceSize vertexStride, core::shared_ptr<Acceleration_Structure_Input_Buffer> vertices);
};

class Acceleration_Structure_Geometry_Instances: public Acceleration_Structure_Geometry
{
public:
  explicit Acceleration_Structure_Geometry_Instances();
  template<class T>
  explicit Acceleration_Structure_Geometry_Instances(core::shared_ptr<Acceleration_Structure_Instance_Buffer<T>> instances) noexcept;
private:
  explicit Acceleration_Structure_Geometry_Instances(core::shared_ptr<Acceleration_Structure_Input_Buffer> instances, core::uint32 instanceCount) noexcept;
};

}

namespace crude::gfx::vk
{

template<class T>
Acceleration_Structure_Geometry_Instances::Acceleration_Structure_Geometry_Instances(core::shared_ptr<Acceleration_Structure_Instance_Buffer<T>> instances) noexcept
  : Acceleration_Structure_Geometry_Instances{ instances, instances->getInstanceCount() } {}

}