module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.shader_stage_create_info;

import crude_engine.core.shared_ptr;

export namespace crude_engine
{

class Shader_Module;

class Shader_Stage_Create_Info : public VkPipelineShaderStageCreateInfo
{
public:
  explicit Shader_Stage_Create_Info(VkShaderStageFlagBits            stage,
                                    Shared_Ptr<const Shader_Module>  shaderModule,
                                    const char*                      pEntrypoint);
private:
  Shared_Ptr<const Shader_Module>  m_shaderModule;
};

}
