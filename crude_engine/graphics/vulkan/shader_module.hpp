#pragma once

#include "../../core/core.hpp"
#include "include_vulkan.hpp"
#include "object.hpp"
#include <memory>
#include <span>
#include <vulkan/vulkan_core.h>

namespace crude_engine
{

class Device;

struct Shader_Module_Create_Info
{
  std::shared_ptr<const Device>  device;
  std::span<const char>          code; 
  explicit Shader_Module_Create_Info(std::shared_ptr<const Device>  device,
                                     std::span<const char>          code);
};

class Shader_Module : public TObject<VkShaderModule>
{
public:
  explicit Shader_Module(const Shader_Module_Create_Info& createInfo);
  ~Shader_Module();
private:
  std::shared_ptr<const Device>  m_device;
};

}
