#pragma once

#include <core/shared_ptr.hpp>
#include <graphics/vulkan/include_vulkan.hpp>

namespace crude_engine
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
