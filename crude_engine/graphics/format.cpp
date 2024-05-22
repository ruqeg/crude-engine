#include <vulkan/vulkan.hpp>

module crude.graphics.vulkan.format;

namespace crude::graphics
{

Format::Format(VkFormat format)
  :
  m_format(format)
{}

bool Format::depth() const
{
  return m_format == VK_FORMAT_D32_SFLOAT 
      || m_format == VK_FORMAT_D16_UNORM;
}

bool Format::stencil() const
{
  return m_format == VK_FORMAT_S8_UINT;
}

bool Format::depthStencil() const
{
  return m_format == VK_FORMAT_D32_SFLOAT_S8_UINT 
      || m_format == VK_FORMAT_D24_UNORM_S8_UINT 
      || m_format == VK_FORMAT_D16_UNORM_S8_UINT;
}

}