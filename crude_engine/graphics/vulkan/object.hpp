#pragma once

#include <graphics/vulkan/vulkan_allocator.hpp>

namespace crude_engine
{

template<class Type, class Allocator = Vulkan_Allocator>
class TObject
{
public:
  Type& getHandle();
  const Type& getHandle() const;
public:
  const VkAllocationCallbacks* getPVkAllocationCallbacks();
protected:
  Type m_handle;
};

#define CRUDE_OBJECT_HANDLE(pData) (pData)->getHandle()

} // namespace crude_engine

#include "object.inl"
