#include <core/memory/free_rbt_allocator.hpp>
#include <core/memory/memory_utils.hpp>

namespace crude_engine
{

template<class T, typename... Args>
T* Free_RBT_Allocator::mnew(Args&&... args) noexcept
{
  return Utility::constructAt(allocate(sizeof(T)), Utility::forward<Args>(args)...);
}

template<class T, typename... Args>
T* Free_RBT_Allocator::mnewArray(std::size_t n, Args&&... args) noexcept
{
  T* ptr = reinterpret_cast<T*>(allocate(sizeof(T) * n));

  for (std::size_t i = 0; i < n; ++i)
  {
    Utility::constructAt(ptr + i, Utility::forward<Args>(args)...);
  }
  return ptr;
}

template<class T>
void Free_RBT_Allocator::mdelete(T* ptr) noexcept
{
  Utility::destructorAt(ptr);
  free(reinterpret_cast<void*>(ptr));
}

template<class T>
void Free_RBT_Allocator::mdeleteArray(std::size_t n, T* ptr) noexcept
{
  for (std::size_t i = 0; i < n; ++i)
  {
    Utility::destructorAt(ptr + i);
  }
  free(reinterpret_cast<void*>(ptr));
}


}