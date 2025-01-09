module;

#include <vulkan/vulkan.h>

export module crude.gfx.vk.acceleration_structure_geometry_triangles;

export import crude.core.std_containers_heap;

export namespace crude::gfx::vk
{
class Acceleration_Structure_Input_Buffer;
}

export namespace crude::gfx::vk
{

class Acceleration_Structure_Geometry_Triangles : public VkAccelerationStructureGeometryKHR
{
public:
  explicit Acceleration_Structure_Geometry_Triangles();
  explicit Acceleration_Structure_Geometry_Triangles(VkFormat vertexFormat, VkDeviceSize vertexStride, core::shared_ptr<Acceleration_Structure_Input_Buffer> vertices);
public:
  core::uint32 primitiveCount;
};

}
