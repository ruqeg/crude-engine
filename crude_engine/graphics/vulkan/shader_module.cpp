#include "shader_module.hpp"
#include "device.hpp"

namespace crude_engine 
{
  
Shader_Module_Create_Info::Shader_Module_Create_Info(std::shared_ptr<const Device>  device,
                                                     std::span<const char>          code)
  :
  device(device),
  code(code)
{}
  
Shader_Module::Shader_Module(const Shader_Module_Create_Info& createInfo)
{
  m_device = createInfo.device;

  VkShaderModuleCreateInfo vkCreateInfo{};
  vkCreateInfo.sType     = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  vkCreateInfo.flags     = 0u;
  vkCreateInfo.pNext     = nullptr;

  vkCreateInfo.codeSize  = static_cast<uint32>(createInfo.code.size());
  vkCreateInfo.pCode     = reinterpret_cast<const uint32*>(createInfo.code.data());

  const VkResult result = vkCreateShaderModule(CRUDE_VULKAN_01_HANDLE(m_device), &vkCreateInfo, nullptr, &m_handle);
  CRUDE_VULKAN_01_HANDLE_RESULT(result, "failed to create shader module");
}

Shader_Module::~Shader_Module()
{
  vkDestroyShaderModule(CRUDE_VULKAN_01_HANDLE(m_device), m_handle, nullptr);
}

}
