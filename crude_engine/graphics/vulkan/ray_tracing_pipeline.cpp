#include <vulkan/vulkan.hpp>

module crude.gfx.vk.ray_tracing_pipeline;

import crude.gfx.vk.pipeline_layout;

namespace crude::gfx::vk
{

Ray_Tracing_Pipeline::Ray_Tracing_Pipeline(core::shared_ptr<Pipeline_Layout>          pipelineLayout,
                                           core::span<const Shader_Stage_Create_Info> shaderStages,
                                           core::span<const Ray_Tracing_Shader_Group> shaderGroups)
  : Pipeline{ VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, pipelineLayout }
{
  //core::vector<VkRayTracingShaderGroupCreateInfoKHR> vkShaderGroups(shaderGroups.begin(), shaderGroups.end());
  //core::vector<VkPipelineShaderStageCreateInfo> vkShaderStages(shaderStages.begin(), shaderStages.end());

  //core::vector< VkRayTracingShaderGroupCreateInfoKHR>
  //VkRayTracingPipelineCreateInfoKHR rayTracingPipelineCI{};
  //rayTracingPipelineCI.sType                        = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR;
  //rayTracingPipelineCI.stageCount                   = static_cast<core::uint32>(vkShaderStages.size());
  //rayTracingPipelineCI.pStages                      = vkShaderStages.data();
  //rayTracingPipelineCI.groupCount                   = static_cast<core::uint32>(vkShaderGroups.size());
  //rayTracingPipelineCI.pGroups                      = vkShaderGroups.data();
  //rayTracingPipelineCI.maxPipelineRayRecursionDepth = 1;
  //rayTracingPipelineCI.layout                       = pipelineLayout->getHandle();
  //VK_CHECK_RESULT(vkCreateRayTracingPipelinesKHR(m_device, VK_NULL_HANDLE, VK_NULL_HANDLE, 1, &rayTracingPipelineCI, nullptr, &m_handle));
}

}
