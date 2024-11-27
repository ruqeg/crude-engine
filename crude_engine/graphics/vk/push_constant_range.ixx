module;

#include <vulkan/vulkan.h>

export module crude.gfx.vk.push_constant_range;

export import crude.core.alias;

export namespace crude::gfx::vk
{

class Push_Constant_Range_Base : public VkPushConstantRange
{
public:
  constexpr Push_Constant_Range_Base(const VkShaderStageFlags flags, const core::uint32 size, const uint32_t offset = 0) noexcept
  {
    this->stageFlags = flags;
    this->offset = offset;
    this->size = size;
  }
};

template<class T>
class Push_Constant_Range : public Push_Constant_Range_Base
{
public:
  constexpr Push_Constant_Range(const VkShaderStageFlags flags, const uint32_t offset = 0) noexcept
    : Push_Constant_Range_Base(flags, sizeof(T), offset) {}
};

template<class T>
class Mesh_Push_Constant_Range : public Push_Constant_Range<T>
{
public:
  constexpr Mesh_Push_Constant_Range(const uint32_t offset = 0) noexcept
    : Push_Constant_Range<T>(VK_SHADER_STAGE_MESH_BIT_EXT, offset) {}
};

template<class T>
class Vertex_Push_Constant_Range : public Push_Constant_Range<T>
{
public:
  constexpr Vertex_Push_Constant_Range(const uint32_t offset = 0) noexcept
    : Push_Constant_Range<T>(VK_SHADER_STAGE_VERTEX_BIT, offset) {}
};

}
