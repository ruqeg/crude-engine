module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.ray_tracing_shader_group;

export import crude.core.alias;

export namespace crude::gfx::vk
{

class Ray_Tracing_Shader_Group : public VkRayTracingShaderGroupCreateInfoKHR
{
protected:
  explicit Ray_Tracing_Shader_Group(VkRayTracingShaderGroupTypeKHR type,
                                    core::uint32                   generalShader, 
                                    core::uint32                   closestHitShader, 
                                    core::uint32                   anyHitShader, 
                                    core::uint32                   intersectionShader)
  : VkRayTracingShaderGroupCreateInfoKHR {
    .sType              = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR,
    .type               = type,
    .generalShader      = generalShader,
    .closestHitShader   = closestHitShader,
    .anyHitShader       = anyHitShader,
    .intersectionShader = intersectionShader, } {}
};

class General_Ray_Tracing_Shader_Group final : public Ray_Tracing_Shader_Group
{
public:
  explicit General_Ray_Tracing_Shader_Group(core::uint32 generalShader)
    : Ray_Tracing_Shader_Group(VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR, generalShader, VK_SHADER_UNUSED_KHR, VK_SHADER_UNUSED_KHR, VK_SHADER_UNUSED_KHR) {}
};

class Triangles_Hit_Ray_Tracing_Shader_Group final : public Ray_Tracing_Shader_Group
{
public:
  explicit Triangles_Hit_Ray_Tracing_Shader_Group(core::uint32 closestHitShader)
    : Ray_Tracing_Shader_Group(VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR, VK_SHADER_UNUSED_KHR, closestHitShader, VK_SHADER_UNUSED_KHR, VK_SHADER_UNUSED_KHR) {}
};

}