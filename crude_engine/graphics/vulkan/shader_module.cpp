#include <graphics/vulkan/shader_module.hpp>
#include <graphics/vulkan/device.hpp>

namespace crude_engine 
{

Shader_Module::Shader_Module(Shared_Ptr<const Device> device, Array_Unsafe<const char> code)
  :
  m_device(device)
{
  VkShaderModuleCreateInfo vkCreateInfo{};
  vkCreateInfo.sType     = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  vkCreateInfo.flags     = 0u;
  vkCreateInfo.pNext     = nullptr;

  vkCreateInfo.codeSize  = static_cast<uint32>(code.size());
  vkCreateInfo.pCode     = reinterpret_cast<const uint32*>(code.data());

  const VkResult result = vkCreateShaderModule(CRUDE_OBJECT_HANDLE(m_device), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  CRUDE_VULKAN_HANDLE_RESULT(result, "failed to create shader module");
}

Shader_Module::~Shader_Module()
{
  vkDestroyShaderModule(CRUDE_OBJECT_HANDLE(m_device), m_handle, getPVkAllocationCallbacks());
}

}
