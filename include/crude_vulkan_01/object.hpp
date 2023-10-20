#pragma once

namespace crude_vulkan_01
{

template<class Type>
class TObject
{
public:
  Type& getHandle();
  const Type& getHandle() const;
protected:
  Type m_handle;
};

} // namespace crude_vulkan_01

#include "object.inl"
