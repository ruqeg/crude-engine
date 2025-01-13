module;

#include <vulkan/vulkan.h>

export module crude.gfx.vk.top_level_acceleration_structure;

export import crude.gfx.vk.acceleration_structure;

export namespace crude::gfx::vk
{

class Top_Level_Acceleration_Structure : public Acceleration_Structure
{
public:
  explicit Top_Level_Acceleration_Structure(core::shared_ptr<Device>                           device,
                                            core::span<const Acceleration_Structure_Geometry>  geometries,
                                            VkAccelerationStructureBuildTypeKHR                buildType,
                                            VkBuildAccelerationStructureFlagsKHR               buildFlags)
    : Acceleration_Structure{ device, VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR, 0u, buildType, buildFlags, geometries } {}
};

}
