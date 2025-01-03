#include <vulkan/vulkan.hpp>

module crude.gfx.vk.shader_stage_create_info;

import crude.gfx.vk.shader_module;

namespace crude::gfx::vk
{

Shader_Stage_Create_Info::Shader_Stage_Create_Info(VkShaderStageFlagBits                  stage,
                                                   core::shared_ptr<const Shader_Module>  shaderModule,
                                                   const char*                            pEntrypoint)
  :
  m_shaderModule(shaderModule)
{
  this->sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  this->pNext               = nullptr;
  this->flags               = 0u;
  this->stage               = stage;
  this->module              = m_shaderModule->getHandle();
  this->pName               = pEntrypoint;
  this->pSpecializationInfo = nullptr;
}

}
