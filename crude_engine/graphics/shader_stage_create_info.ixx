module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.shader_stage_create_info;

import crude.core.std_containers_heap;

export namespace crude::graphics
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

}
