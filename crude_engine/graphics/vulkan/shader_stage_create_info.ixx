module;

#include <vulkan/vulkan.hpp>

export module crude.gfx.vk.shader_stage_create_info;

import crude.core.std_containers_heap;

export namespace crude::gfx::vk
{

class Shader_Module;

class Shader_Stage_Create_Info : public VkPipelineShaderStageCreateInfo
{
public:
  explicit Shader_Stage_Create_Info(VkShaderStageFlagBits                  stage,
                                    core::shared_ptr<const Shader_Module>  shaderModule,
                                    const char*                            pEntrypoint);
private:
  core::shared_ptr<const Shader_Module>  m_shaderModule;
};

class Task_Shader_Stage_Create_Info : public Shader_Stage_Create_Info
{
public:
  explicit Task_Shader_Stage_Create_Info(core::shared_ptr<const Shader_Module> shaderModule, const char* pEntrypoint)
    : Shader_Stage_Create_Info(VK_SHADER_STAGE_TASK_BIT_EXT, shaderModule, pEntrypoint) {}
};

class Mesh_Shader_Stage_Create_Info : public Shader_Stage_Create_Info
{
public:
  explicit Mesh_Shader_Stage_Create_Info(core::shared_ptr<const Shader_Module> shaderModule, const char* pEntrypoint)
    : Shader_Stage_Create_Info(VK_SHADER_STAGE_MESH_BIT_EXT, shaderModule, pEntrypoint) {}
};

class Fragment_Shader_Stage_Create_Info : public Shader_Stage_Create_Info
{
public:
  explicit Fragment_Shader_Stage_Create_Info(core::shared_ptr<const Shader_Module> shaderModule, const char* pEntrypoint)
    : Shader_Stage_Create_Info(VK_SHADER_STAGE_FRAGMENT_BIT, shaderModule, pEntrypoint) {}
};

class Miss_Shader_Stage_Create_Info : public Shader_Stage_Create_Info
{
public:
  explicit Miss_Shader_Stage_Create_Info(core::shared_ptr<const Shader_Module> shaderModule, const char* pEntrypoint)
    : Shader_Stage_Create_Info(VK_SHADER_STAGE_MISS_BIT_KHR, shaderModule, pEntrypoint) {}
};

class Raygen_Shader_Stage_Create_Info : public Shader_Stage_Create_Info
{
public:
  explicit Raygen_Shader_Stage_Create_Info(core::shared_ptr<const Shader_Module> shaderModule, const char* pEntrypoint)
    : Shader_Stage_Create_Info(VK_SHADER_STAGE_RAYGEN_BIT_KHR, shaderModule, pEntrypoint) {}
};

class Closest_Hit_Shader_Stage_Create_Info : public Shader_Stage_Create_Info
{
public:
  explicit Closest_Hit_Shader_Stage_Create_Info(core::shared_ptr<const Shader_Module> shaderModule, const char* pEntrypoint)
    : Shader_Stage_Create_Info(VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR, shaderModule, pEntrypoint) {}
};

}
