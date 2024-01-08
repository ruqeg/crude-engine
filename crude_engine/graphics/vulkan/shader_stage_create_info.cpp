#include <graphics/vulkan/shader_stage_create_info.hpp>
#include <graphics/vulkan/shader_module.hpp>

namespace crude_engine
{

Shader_Stage_Create_Info::Shader_Stage_Create_Info(VkShaderStageFlagBits            stage,
                                                   Shared_Ptr<const Shader_Module>  shaderModule,
                                                   const char*                      pEntrypoint)
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
