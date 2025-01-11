module;

#include <vulkan/vulkan.h>

export module crude.gfx.vk.acceleration_structure_instance;

export import crude.gfx.vk.buffer;

export namespace crude::gfx::vk
{

struct Transform_Matrix : public VkTransformMatrixKHR
{
  Transform_Matrix() noexcept
    : VkTransformMatrixKHR{
      {
        { 1, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 1, 0 }
      }
    } {}
};

struct Acceleration_Structure_Instance : public VkAccelerationStructureInstanceKHR
{
  Acceleration_Structure_Instance() noexcept
    : VkAccelerationStructureInstanceKHR{
      .transform = Transform_Matrix{},
      .instanceCustomIndex = 0u,
      .mask = 0xff,
      .instanceShaderBindingTableRecordOffset = 0u,
      .flags = 0u,
      .accelerationStructureReference = 0u,
    } {}
};

}