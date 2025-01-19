// !TODO Pipeline_Cahe
module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.pipeline;

import crude.gfx.vk.vulkan_object;
export import crude.gfx.vk.shader_stage_create_info;
export import crude.gfx.vk.vertex_input_state_create_info;
export import crude.gfx.vk.input_assembly_state_create_info;
export import crude.gfx.vk.viewport_state_create_info;
export import crude.gfx.vk.rasterization_state_create_info;
export import crude.gfx.vk.multisample_state_create_info;
export import crude.gfx.vk.depth_stencil_state_create_info;
export import crude.gfx.vk.color_blend_state_create_info;
export import crude.gfx.vk.dynamic_state_create_info;
export import crude.gfx.vk.tessellation_state_create_info;
export import crude.core.std_containers_stack;
export import crude.core.std_containers_heap;
export import crude.gfx.vk.render_pass;

export namespace crude::gfx::vk
{

class Device;
class Pipeline_Layout;

class Pipeline : public Vulkan_Object<VkPipeline>
{
protected:
  explicit Pipeline(VkPipelineBindPoint bindPoint, core::shared_ptr<Pipeline_Layout> pipelineLayout);
  ~Pipeline();
public:
  core::shared_ptr<Pipeline_Layout> getPipelineLayout() const;
  core::shared_ptr<const Device> getDevice() const;
  VkPipelineBindPoint getBindPoint() const;
protected:
  core::shared_ptr<const Device>    m_device;
  core::shared_ptr<Pipeline_Layout> m_pipelineLayout;
  const VkPipelineBindPoint         m_bindPoint;
};

}