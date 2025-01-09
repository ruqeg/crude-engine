module;

#include <vulkan/vulkan.h>

export module crude.gfx.vk.bottom_level_acceleration_structure;

export import crude.gfx.vk.acceleration_structure;

export namespace crude::gfx::vk
{
class Device;
class Device;
}

export namespace crude::gfx::vk
{

class Bottom_Level_Acceleration_Structure : public Acceleration_Structure
{
public:
  explicit Bottom_Level_Acceleration_Structure(core::shared_ptr<Device>                                     device,
                                               core::span<const Acceleration_Structure_Geometry_Triangles>  geometries,
                                               VkAccelerationStructureBuildTypeKHR                          buildType,
                                               VkBuildAccelerationStructureFlagsKHR                         buildFlags)
    : Acceleration_Structure{ device, VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR, 0u, buildType, buildFlags, geometries } {}
};

}
