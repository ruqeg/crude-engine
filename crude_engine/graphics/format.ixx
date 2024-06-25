module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.format;

export namespace crude::graphics
{

class Format
{
public:
  constexpr Format(VkFormat format) noexcept 
    : m_format(format) {}
  bool depth() const
  {
    return m_format == VK_FORMAT_D32_SFLOAT 
        || m_format == VK_FORMAT_D16_UNORM;
  }
  bool stencil() const
  {
    return m_format == VK_FORMAT_S8_UINT;
  }
  bool depthStencil() const
  {
    return m_format == VK_FORMAT_D32_SFLOAT_S8_UINT 
        || m_format == VK_FORMAT_D24_UNORM_S8_UINT 
        || m_format == VK_FORMAT_D16_UNORM_S8_UINT;
  }
  constexpr operator VkFormat() const noexcept { return m_format; }
private:
  VkFormat m_format;
};

}
