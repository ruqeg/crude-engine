module;

#include <vulkan/vulkan.h>

export module crude.gfx.vk.acceleration_structure_geometry_triangles;

export import crude.core.std_containers_heap;
import crude.gfx.vk.acceleration_structure_input_buffer;
import crude.gfx.vk.constants;
import crude.gfx.vk.utils;
import crude.gfx.vk.format;
import crude.gfx.vk.acceleration_structure_instance_buffer;

export namespace crude::gfx::vk
{
class Acceleration_Structure_Input_Buffer;
}

export namespace crude::gfx::vk
{

struct Acceleration_Structure_Geometry : public VkAccelerationStructureGeometryKHR
{
  Acceleration_Structure_Geometry(VkGeometryTypeKHR geometryType, VkGeometryFlagsKHR flags = 0) noexcept
    : primitiveCount{ 0u }
  {
    this->sType        = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
    this->pNext        = nullptr;
    this->geometryType = geometryType;
    this->flags        = flags;
  }
  core::uint32 primitiveCount;
};

struct Acceleration_Structure_Geometry_Triangles : public Acceleration_Structure_Geometry
{
  explicit Acceleration_Structure_Geometry_Triangles()
    : Acceleration_Structure_Geometry{ VK_GEOMETRY_TYPE_TRIANGLES_KHR }
  {
    this->geometry.triangles.sType                       = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
    this->geometry.triangles.pNext                       = nullptr;
    this->geometry.triangles.vertexFormat                = VK_FORMAT_UNDEFINED;
    this->geometry.triangles.vertexData.deviceAddress    = cDeviceAddressNull;
    this->geometry.triangles.vertexStride                = 0u;
    this->geometry.triangles.maxVertex                   = 0u;
    this->geometry.triangles.indexType                   = VK_INDEX_TYPE_NONE_KHR;
    this->geometry.triangles.indexData.deviceAddress     = cDeviceAddressNull;
    this->geometry.triangles.transformData.deviceAddress = cDeviceAddressNull;
  }
  explicit Acceleration_Structure_Geometry_Triangles(VkFormat vertexFormat, VkDeviceSize vertexStride, core::shared_ptr<Acceleration_Structure_Input_Buffer> vertices)
    : Acceleration_Structure_Geometry_Triangles{}
  {    
    this->geometry.triangles.vertexFormat  = vertexFormat;
    this->geometry.triangles.vertexData    = getAddress(vertices);
    this->geometry.triangles.vertexStride  = vertexStride;
    this->geometry.triangles.maxVertex     = static_cast<core::uint32>(vertices->getSize() / geometry.triangles.vertexStride) - 1;
    this->geometry.triangles.transformData = VkDeviceOrHostAddressConstKHR{};
    this->flags                            = 0;

    primitiveCount = (geometry.triangles.maxVertex + 1) / 3;
  }
};

struct Acceleration_Structure_Geometry_Instances : public Acceleration_Structure_Geometry
{
  explicit Acceleration_Structure_Geometry_Instances()
    : Acceleration_Structure_Geometry{ VK_GEOMETRY_TYPE_INSTANCES_KHR }
  {
    this->geometry.instances.sType              = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
    this->geometry.instances.pNext              = nullptr;
    this->geometry.instances.arrayOfPointers    = VK_FALSE;
    this->geometry.instances.data.deviceAddress = cDeviceAddressNull;
  }
  template<class T>
  explicit Acceleration_Structure_Geometry_Instances(core::shared_ptr<Acceleration_Structure_Instance_Buffer<T>> instances) noexcept
    : Acceleration_Structure_Geometry_Instances{}
  {
    this->geometry.instances.sType           = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
    this->geometry.instances.pNext           = nullptr;
    this->geometry.instances.arrayOfPointers = VK_FALSE;
    this->geometry.instances.data            = getAddress(instances);
    
    primitiveCount = instances->getInstanceCount();
  }
};

}
