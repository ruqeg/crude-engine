module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.ray_tracing_pipeline;

export import crude.gfx.vk.pipeline;
export import crude.gfx.vk.ray_tracing_shader_group;
export import crude.gfx.vk.shader_stage_create_info;

export namespace crude::gfx::vk
{

class Pipeline_Layout;

class Ray_Tracing_Pipeline : public Pipeline
{
public:
  explicit Ray_Tracing_Pipeline(core::shared_ptr<Pipeline_Layout>          pipelineLayout,
                                core::span<const Shader_Stage_Create_Info> shaderStages,
                                core::span<const Ray_Tracing_Shader_Group> shaderGroups);
};

}
