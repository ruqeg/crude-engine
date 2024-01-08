#pragma once

#include <core/data_structures/shared_ptr.hpp>
#include <core/data_structures/array_unsafe.hpp>
#include <graphics/vulkan/object.hpp>
#include <graphics/vulkan/include_vulkan.hpp>

namespace crude_engine
{

class Device;

class Shader_Module : public TObject<VkShaderModule>
{
public:
  explicit Shader_Module(Shared_Ptr<const Device> device, Array_Unsafe<const char> code);
  ~Shader_Module();
private:
  Shared_Ptr<const Device>  m_device;
};

}
