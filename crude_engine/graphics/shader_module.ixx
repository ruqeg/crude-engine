module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.shader_module;

import crude.core.shared_ptr;
import crude.core.std_containers_stack;
import crude.graphics.vulkan_object;

export namespace crude::graphics
{

class Device;

class Shader_Module : public Vulkan_Object<VkShaderModule>
{
public:
  explicit Shader_Module(core::Shared_Ptr<const Device> device, core::span<const char> code);
  ~Shader_Module();
private:
  core::Shared_Ptr<const Device>  m_device;
};

}
