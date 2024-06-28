module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.shader_module;

import crude.core.std_containers_heap;
import crude.core.std_containers_stack;
import crude.graphics.vulkan_object;

export namespace crude::graphics
{

class Device;

using Spirv_Word = uint32_t;

class Shader_Module : public Vulkan_Object<VkShaderModule>
{
public:
  explicit Shader_Module(core::shared_ptr<const Device> device, core::span<const Spirv_Word> code);
  ~Shader_Module();
private:
  core::shared_ptr<const Device>  m_device;
};

}
