#pragma once

#include <core/shared_ptr.hpp>
#include <core/std_containers.hpp>
#include <graphics/vulkan/object.hpp>
#include <graphics/vulkan/include_vulkan.hpp>

namespace crude_engine
{

class Device;

class Shader_Module : public TObject<VkShaderModule>
{
public:
  explicit Shader_Module(Shared_Ptr<const Device> device, span<const char> code);
  ~Shader_Module();
private:
  Shared_Ptr<const Device>  m_device;
};

}
