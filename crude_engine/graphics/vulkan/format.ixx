module;

#include <vulkan/vulkan.hpp>

export module crude_engine.graphics.vulkan.format;

namespace crude_engine
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