#include <vulkan/vulkan.hpp>

module crude.gfx.vk.ray_tracing_pipeline;

namespace crude::gfx::vk
{

Ray_Tracing_Pipeline::Ray_Tracing_Pipeline()
  : Pipeline{ }
{
  /*
	  Setup ray tracing shader groups
  */
  std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

  // Ray generation group
  {
	shaderStages.push_back(loadShader(getShadersPath() + "raytracingbasic/raygen.rgen.spv", VK_SHADER_STAGE_RAYGEN_BIT_KHR));
	VkRayTracingShaderGroupCreateInfoKHR shaderGroup{};
	shaderGroup.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
	shaderGroup.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
	shaderGroup.generalShader = static_cast<uint32_t>(shaderStages.size()) - 1;
	shaderGroup.closestHitShader = VK_SHADER_UNUSED_KHR;
	shaderGroup.anyHitShader = VK_SHADER_UNUSED_KHR;
	shaderGroup.intersectionShader = VK_SHADER_UNUSED_KHR;
	shaderGroups.push_back(shaderGroup);
  }

  // Miss group
  {
	shaderStages.push_back(loadShader(getShadersPath() + "raytracingbasic/miss.rmiss.spv", VK_SHADER_STAGE_MISS_BIT_KHR));
	VkRayTracingShaderGroupCreateInfoKHR shaderGroup{};
	shaderGroup.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
	shaderGroup.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
	shaderGroup.generalShader = static_cast<uint32_t>(shaderStages.size()) - 1;
	shaderGroup.closestHitShader = VK_SHADER_UNUSED_KHR;
	shaderGroup.anyHitShader = VK_SHADER_UNUSED_KHR;
	shaderGroup.intersectionShader = VK_SHADER_UNUSED_KHR;
	shaderGroups.push_back(shaderGroup);
  }

  // Closest hit group
  {
	shaderStages.push_back(loadShader(getShadersPath() + "raytracingbasic/closesthit.rchit.spv", VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR));
	VkRayTracingShaderGroupCreateInfoKHR shaderGroup{};
	shaderGroup.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
	shaderGroup.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR;
	shaderGroup.generalShader = VK_SHADER_UNUSED_KHR;
	shaderGroup.closestHitShader = static_cast<uint32_t>(shaderStages.size()) - 1;
	shaderGroup.anyHitShader = VK_SHADER_UNUSED_KHR;
	shaderGroup.intersectionShader = VK_SHADER_UNUSED_KHR;
	shaderGroups.push_back(shaderGroup);
  }

  /*
	  Create the ray tracing pipeline
  */
  VkRayTracingPipelineCreateInfoKHR rayTracingPipelineCI{};
  rayTracingPipelineCI.sType                        = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR;
  rayTracingPipelineCI.stageCount                   = static_cast<uint32_t>(shaderStages.size());
  rayTracingPipelineCI.pStages                      = shaderStages.data();
  rayTracingPipelineCI.groupCount                   = static_cast<uint32_t>(shaderGroups.size());
  rayTracingPipelineCI.pGroups                      = shaderGroups.data();
  rayTracingPipelineCI.maxPipelineRayRecursionDepth = 1;
  rayTracingPipelineCI.layout                       = pipelineLayout;
  VK_CHECK_RESULT(vkCreateRayTracingPipelinesKHR(device, VK_NULL_HANDLE, VK_NULL_HANDLE, 1, &rayTracingPipelineCI, nullptr, &m_handle));
}

}
