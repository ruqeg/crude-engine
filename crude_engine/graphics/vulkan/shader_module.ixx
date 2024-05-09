module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.shader_module;

import crude_engine.core.shared_ptr;
import crude_engine.core.std_containers_stack;
import crude_engine.graphics.vulkan.vulkan_object;

export namespace crude_engine
{

class Device;

class Shader_Module : public Vulkan_Object<VkShaderModule>
{
public:
  explicit Shader_Module(Shared_Ptr<const Device> device, span<const char> code);
  ~Shader_Module();
private:
  Shared_Ptr<const Device>  m_device;
};

}
