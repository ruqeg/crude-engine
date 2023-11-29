#pragma once

#include "../../core/core.hpp"
#include "include_vulkan.hpp"

namespace crude_vulkan_01
{

class Shader_Module;

class Shader_Stage_Create_Info : public VkPipelineShaderStageCreateInfo
{
public:
  explicit Shader_Stage_Create_Info(VkShaderStageFlagBits                 stage,
                                    std::shared_ptr<const Shader_Module>  shaderModule,
                                    const char*                           entrypoint);
  Shader_Stage_Create_Info(const Shader_Stage_Create_Info& other);
  Shader_Stage_Create_Info(Shader_Stage_Create_Info&& other) noexcept;
  Shader_Stage_Create_Info& operator=(const Shader_Stage_Create_Info& other);
  Shader_Stage_Create_Info& operator=(Shader_Stage_Create_Info&& other) noexcept;
  ~Shader_Stage_Create_Info();
private:
  void copy(VkShaderStageFlagBits                 stage,
            std::shared_ptr<const Shader_Module>  shaderModule,
            const char*                           entrypoint);
  void move(Shader_Stage_Create_Info& other);
private:
  std::shared_ptr<const Shader_Module>  m_shaderModule;
};

}
