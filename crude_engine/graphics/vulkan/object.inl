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
const VkAllocationCallbacks* TObject<Type, Allocator>::getPVkAllocationCallbacks()
{
  // !TODO
  static VkAllocationCallbacks instance = Allocator();
  return nullptr;
}

} // namespace crude_engine
