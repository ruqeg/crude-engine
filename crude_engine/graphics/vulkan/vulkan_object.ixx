module;

#include <vulkan/vulkan_core.h>

export module crude.gfx.vk.vulkan_object;

import crude.gfx.vk.vulkan_allocator;

export namespace crude::gfx::vk
{

template<class Type, class Allocator = Vulkan_Allocator>
class Vulkan_Object
{
public:
  Type& getHandle();
  const Type& getHandle() const;
public:
  const VkAllocationCallbacks* getPVkAllocationCallbacks();
protected:
  Type m_handle;
};

template<class Type, class Allocator>
Type& Vulkan_Object<Type, Allocator>::getHandle()
{
  return m_handle;
}

template<class Type, class Allocator>
const Type& Vulkan_Object<Type, Allocator>::getHandle() const
{
  return m_handle;
}

template<class Type, class Allocator>
const VkAllocationCallbacks* Vulkan_Object<Type, Allocator>::getPVkAllocationCallbacks()
{
  static VkAllocationCallbacks instance = Allocator();
  return nullptr;
}

} // namespace crude
