#include <core/memory/free_rbt_allocator.hpp>

namespace crude_engine
{

template<class T, typename... Args>
T* Free_RBT_Allocator::mnew(std::size_t n, Args&&... args) noexcept
{
  return new (allocate(sizeof(T) * n)) T(args...);
}

template<class T>
void Free_RBT_Allocator::mdelete(T* ptr) noexcept
{
  ptr->~T();
  free(reinterpret_cast<void*>(ptr));
}


}