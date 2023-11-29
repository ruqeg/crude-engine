#pragma once

#include "include_vulkan.hpp"

namespace crude_vulkan_01
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