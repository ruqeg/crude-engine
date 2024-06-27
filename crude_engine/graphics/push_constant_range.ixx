module;

#include <vulkan/vulkan.h>

export module crude.graphics.push_constant_range;

export import crude.core.alias;

export namespace crude::graphics
{

struct Push_Constant_Range_Base : public VkPushConstantRange
{
  constexpr Push_Constant_Range_Base(const VkShaderStageFlags flags, const core::uint32 size, const uint32_t offset = 0) noexcept
  {
    this->stageFlags = flags;
    this->offset = offset;
    this->size = size;
  }
};

namespace push
{

template<class T>
struct Push_Constant_Range : public Push_Constant_Range_Base
{
  constexpr Push_Constant_Range(const VkShaderStageFlags flags, const uint32_t offset = 0) noexcept
    : Push_Constant_Range_Base(flags, sizeof(T), offset) {}
};

template<class T>
struct Vertex_Push_Constant_Range : public Push_Constant_Range<T>
{
  constexpr Vertex_Push_Constant_Range(const uint32_t offset = 0) noexcept
    : Push_Constant_Range<T>(VK_SHADER_STAGE_VERTEX_BIT, offset) {}
};

}

}
