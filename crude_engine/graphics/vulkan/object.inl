#include "object.hpp"
namespace crude_engine
{

template<class Type, class Allocator>
Type& TObject<Type, Allocator>::getHandle()
{
  return m_handle;
}

template<class Type, class Allocator>
const Type& TObject<Type, Allocator>::getHandle() const
{
  return m_handle;
}

template<class Type, class Allocator>
const VkAllocationCallbacks& TObject<Type, Allocator>::getVkAllocationCallbacks()
{
  static VkAllocationCallbacks instance = Allocator();
  return instance;
}

} // namespace crude_engine
