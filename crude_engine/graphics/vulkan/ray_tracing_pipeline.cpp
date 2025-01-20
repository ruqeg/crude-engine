#include <vulkan/vulkan.hpp>

module crude.gfx.vk.ray_tracing_pipeline;

import crude.gfx.vk.device;
import crude.gfx.vk.pipeline_layout;
import crude.gfx.vk.vulkan_utils;
import crude.gfx.vk.extension;

namespace crude::gfx::vk
{

Ray_Tracing_Pipeline::Ray_Tracing_Pipeline(core::shared_ptr<Pipeline_Layout>          pipelineLayout,
                                           core::span<const Shader_Stage_Create_Info> shaderStages,
                                           core::span<const Ray_Tracing_Shader_Group> shaderGroups)
  : Pipeline{ VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, pipelineLayout }
{
  core::vector<VkRayTracingShaderGroupCreateInfoKHR> vkShaderGroups(shaderGroups.begin(), shaderGroups.end());
  core::vector<VkPipelineShaderStageCreateInfo> vkShaderStages(shaderStages.begin(), shaderStages.end());

  VkRayTracingPipelineCreateInfoKHR vkCreateInfo{};
  vkCreateInfo.sType                        = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR;
  vkCreateInfo.stageCount                   = static_cast<core::uint32>(vkShaderStages.size());
  vkCreateInfo.pStages                      = vkShaderStages.data();
  vkCreateInfo.groupCount                   = static_cast<core::uint32>(vkShaderGroups.size());
  vkCreateInfo.pGroups                      = vkShaderGroups.data();
  vkCreateInfo.maxPipelineRayRecursionDepth = 1;
  vkCreateInfo.layout                       = pipelineLayout->getHandle();

  auto vkCreateRayTracingPipelinesKHR = getDeviceExtension<PFN_vkCreateRayTracingPipelinesKHR>(m_device);
  if (vkCreateRayTracingPipelinesKHR)
  {
    vulkanHandleResult(
      vkCreateRayTracingPipelinesKHR(m_device->getHandle(), VK_NULL_HANDLE, VK_NULL_HANDLE, 1, &vkCreateInfo, nullptr, &m_handle),
      "Failed create ray tracing pipeline!");
  }
}

}
