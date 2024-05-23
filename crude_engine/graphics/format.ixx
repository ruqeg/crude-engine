module;

#include <vulkan/vulkan.hpp>

export module crude.graphics.format;

export namespace crude::graphics
{

class Format
{
public:
  explicit Format(VkFormat format);
  bool depth() const;
  bool stencil() const;
  bool depthStencil() const;
private:
  VkFormat m_format;
};

}