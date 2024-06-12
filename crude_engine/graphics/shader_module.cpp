#include <vulkan/vulkan.hpp>

module crude.graphics.shader_module;

import crude.graphics.device;
import crude.graphics.vulkan_utils;

namespace crude::graphics
{

Shader_Module::Shader_Module(core::shared_ptr<const Device> device, core::span<const char> code)
  :
  m_device(device)
{
  VkShaderModuleCreateInfo vkCreateInfo{};
  vkCreateInfo.sType     = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  vkCreateInfo.flags     = 0u;
  vkCreateInfo.pNext     = nullptr;

  vkCreateInfo.codeSize  = static_cast<core::uint32>(code.size());
  vkCreateInfo.pCode     = reinterpret_cast<const core::uint32*>(code.data());

  const VkResult result = vkCreateShaderModule(m_device->getHandle(), &vkCreateInfo, getPVkAllocationCallbacks(), &m_handle);
  vulkanHandleResult(result, "failed to create shader module");
}

Shader_Module::~Shader_Module()
{
  vkDestroyShaderModule(m_device->getHandle(), m_handle, getPVkAllocationCallbacks());
}

}
