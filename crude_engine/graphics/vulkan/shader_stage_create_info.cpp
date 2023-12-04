#include "shader_stage_create_info.hpp"
#include "shader_module.hpp"

namespace crude_engine
{

Shader_Stage_Create_Info::Shader_Stage_Create_Info(VkShaderStageFlagBits                 stage,
                                                   std::shared_ptr<const Shader_Module>  shaderModule,
                                                   const char*                           entrypoint)
{
  copy(stage, shaderModule, entrypoint);
}

Shader_Stage_Create_Info::Shader_Stage_Create_Info(const Shader_Stage_Create_Info& other)
{
  copy(other.stage, other.m_shaderModule, other.pName);
}

Shader_Stage_Create_Info::Shader_Stage_Create_Info(Shader_Stage_Create_Info&& other) noexcept
{
  move(other);
}

Shader_Stage_Create_Info& Shader_Stage_Create_Info::operator=(const Shader_Stage_Create_Info& other)
{
  copy(other.stage, other.m_shaderModule, other.pName);
  return *this;
}

Shader_Stage_Create_Info& Shader_Stage_Create_Info::operator=(Shader_Stage_Create_Info&& other) noexcept
{
  move(other);
  return *this;
}

Shader_Stage_Create_Info::~Shader_Stage_Create_Info()
{
  //=========
  // !FREE
  if (this->pName)  CRUDE_VULKAN_01_DELETE[] this->pName;
  //=========
}

void Shader_Stage_Create_Info::copy(VkShaderStageFlagBits stage, std::shared_ptr<const Shader_Module> shaderModule, const char* entrypoint)
{ 
  m_shaderModule = shaderModule;

  this->sType                = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  this->pNext                = nullptr;
  this->flags                = 0u;
  this->stage                = stage;
  this->module               = m_shaderModule->getHandle();
  //=========
  // !MALLOC
  this->pName                = CRUDE_VULKAN_01_NEW_COPY_STRING(entrypoint);
  //=========
  this->pSpecializationInfo  = nullptr; // !TODO
}

void Shader_Stage_Create_Info::move(Shader_Stage_Create_Info& other)
{
  m_shaderModule = other.m_shaderModule;

  this->sType                = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  this->pNext                = nullptr;
  this->flags                = 0u;
  this->stage                = stage;
  this->module               = m_shaderModule->getHandle();
  this->pName                = other.pName;
  this->pSpecializationInfo  = nullptr; // !TODO

  other.pName = nullptr;
}

}
